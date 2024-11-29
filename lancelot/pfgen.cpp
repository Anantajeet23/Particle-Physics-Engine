#include "pfgen.h"
#include <iostream>
#include <cmath>

namespace lancelot {

    // ParticleForceRegistry
    void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
        ParticleForce registration = { particle, fg };
        registrations.push_back(registration);
    }


    void ParticleForceRegistry::clear() {
        registrations.clear();
    }

    void ParticleForceRegistry::updateForces(real duration) {
        Registry::iterator i = registrations.begin();
        for (; i != registrations.end(); i++)
        {
            i->fg->update(i->Particle, duration);
        }
        

    }

    // ParticleGravity
    ParticleGravity::ParticleGravity(const Vector3d& gravity) : gravity(gravity) {}

    void ParticleGravity::update(Particle* particle, real duration) {
        if (particle->getInverseMass() == 0) return; // Skip immovable particles
        particle->addForce(gravity * particle->getMass());
    }

    // ParticlePairwiseGravity
    ParticlePairwiseGravity::ParticlePairwiseGravity(real G, std::vector<Particle*>* particles) 
        : G(G), particles(particles) {}

    void ParticlePairwiseGravity::update(Particle* particle, real duration) {
        Vector3d totalForce;

        for (Particle* other : *particles) {
            if (particle == other || other->getInverseMass() == 0) continue;

            Vector3d displacement = other->getPosition() - particle->getPosition();
            real distanceSquared = displacement.squaredMagnitude();
            if (distanceSquared < 1e-6) distanceSquared = 1e-6;

            real forceMagnitude = G * particle->getMass() * other->getMass() / distanceSquared;
            Vector3d force = displacement * (forceMagnitude / sqrt(distanceSquared));
            totalForce += force;
        }

        particle->addForce(totalForce);
    }

    // ParticleCOMGravity
    ParticleCOMGravity::ParticleCOMGravity(real G, std::vector<Particle*>* particles, const Vector3d& center) 
        : G(G), particles(particles), center(center) {}

    void ParticleCOMGravity::update(Particle* particle, real duration) {
        Vector3d com(0, 0, 0);
        real totalMass = 0;

        for (Particle* other : *particles) {
            com += (other->getPosition() - center) * other->getMass();
            totalMass += other->getMass();
        }

        if (totalMass > 0) {
            com *= 1.0 / totalMass;
            com += center;
        }

        Vector3d displacement = com - particle->getPosition();
        real distance = displacement.magnitude();
        if (distance < 1e-3) return;

        displacement.normalize();
        real forceMagnitude = G * particle->getMass() * totalMass / (distance * distance);
        Vector3d force = displacement * forceMagnitude;
        particle->addForce(force);
    }

    // ParticleDrag
    ParticleDrag::ParticleDrag(real k1, real k2) : k1(k1), k2(k2) {}

    void ParticleDrag::update(Particle* particle, real duration) {
        Vector3d velocity = particle->getVelocity();
        real dragCoefficient = velocity.magnitude();
        dragCoefficient = k1 * dragCoefficient + k2 * dragCoefficient * dragCoefficient;
        velocity.normalize();
        particle->addForce(velocity * -dragCoefficient);
    }

    // ParticleString
    ParticleString::ParticleString(Particle* other, real springConst, real restLength)
        : other(other), springConstant(springConst), restLength(restLength) {}

    void ParticleString::update(Particle* particle, real duration) {
        Vector3d force = particle->getPosition() - other->getPosition();
        real magnitude = force.magnitude();
        magnitude = -springConstant * std::abs(magnitude - restLength);
        force.normalize();
        particle->addForce(force * magnitude);
    }

    // ParticleAnchoredString
    ParticleAnchoredString::ParticleAnchoredString(Vector3d* anchor, real springConst, real restLength)
        : anchor(anchor), springConstant(springConst), restLength(restLength) {}

    void ParticleAnchoredString::update(Particle* particle, real duration) {
        Vector3d force = particle->getPosition() - *anchor;
        real magnitude = force.magnitude();
        magnitude = -springConstant * std::abs(magnitude - restLength);
        force.normalize();
        particle->addForce(force * magnitude);
    }

    // ParticleBungee
    ParticleBungee::ParticleBungee(Particle* other, real springConst, real restLength)
        : other(other), springConstant(springConst), restLength(restLength) {}

    void ParticleBungee::update(Particle* particle, real duration) {
        Vector3d force = particle->getPosition() - other->getPosition();
        real magnitude = force.magnitude();
        if (magnitude <= restLength) return;
        magnitude = -springConstant * std::abs(magnitude - restLength);
        force.normalize();
        particle->addForce(force * magnitude);
    }

    // ParticleAnchoredBungee
    ParticleAnchoredBungee::ParticleAnchoredBungee(Vector3d* anchor, real springConst, real restLength)
        : anchor(anchor), springConstant(springConst), restLength(restLength) {}

    void ParticleAnchoredBungee::update(Particle* particle, real duration) {
        Vector3d force = particle->getPosition() - *anchor;
        real magnitude = force.magnitude();
        if (magnitude <= restLength) return;
        magnitude = -springConstant * std::abs(magnitude - restLength);
        force.normalize();
        particle->addForce(force * magnitude);
    }

    // ParticleBuoyancy
    ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real liquidHeight, real liquidDensity)
        : maxDepth(maxDepth), volume(volume), liquidHeight(liquidHeight), liquidDensity(liquidDensity) {}

    void ParticleBuoyancy::update(Particle* particle, real duration) {
        real depth = particle->getPosition().y;
        if (depth >= liquidHeight + maxDepth) return;

        Vector3d force(0, 0, 0);
        if (depth <= liquidHeight - maxDepth) {
            force.y = liquidDensity * volume;
        } else {
            force.y = liquidDensity * volume * (depth - maxDepth - liquidHeight) / (2 * maxDepth);
        }



        particle->addForce(force * particle->getDamping());
        
    }

}