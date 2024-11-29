#include "particle.h"
#include <assert.h>

using namespace lancelot;
namespace lancelot{

        

        /* Setters and getters */
        real Particle::getMass() const{
            return Mass;
        }

        real Particle::getInverseMass() const {
            return inverseMass;
        }

        real Particle::getRadius() const{
            return radius;
        }

        Vector3d Particle::getForceAccum() const {
            return forceAccum;
        }

        Vector3d Particle::getPosition() const {
            return position;
        }

        Vector3d Particle::getVelocity() const {
            return velocity;
        }

        Vector3d Particle::getAcceleration() const {
            return acceleration;
        }

        real Particle::getDamping() const {
            return damping;
        }


        /* used to clear the forces at the end of the integrator */
        void Particle::clearAccumulator(){
            forceAccum *= 0;
        }

        /* used to add forces for the next iteration of the integrator */
        void Particle::addForce(const Vector3d& force)  {
            forceAccum += force;
        }

        void Particle::setMass(real m){
            /** Sets both Mass then calulates and sets inverse mass
             * Mass should not be zero, leads to strange behaviour (infinite acceleration)
             */
            this->Mass = m;
            this->inverseMass = 1/m;
        }

        void Particle::setRadius(real r) {
            this->radius = r;
        }

        void Particle::setPosition(const Vector3d& new_position)  {
            position = new_position;
        }

        void Particle::Particle::setVelocity(const Vector3d& new_velocity)  {
            velocity = new_velocity;
        }

        void Particle::setAcceleration(const Vector3d& new_acceleration)  {
            acceleration = new_acceleration;
        }

        void Particle::setDamping(real damp)  {
            damping = damp;
        }


        /* Vector representing gravity, THE Y-AXIS REPRESENTS UP AND DOWN */
        Vector3d g; 

        /**
        * Integrates the particle forward in time by the given amount.
        * This function uses a Newton-Euler integration method, which is a
        * linear approximation of the correct integral. For this reason it
        * may be inaccurate in some cases.
        */
        void Particle::integrate(real duration){
            
        assert(duration > 0.0);

        // Update linear position using p' = p + velocity * time + acceleration * 0.5 * time * time .
        position.addScaledVector(velocity, duration);
        position.addScaledVector(acceleration, duration*duration*0.5);

        // Work out the acceleration from the force.
        Vector3d resultingAcc = acceleration;
        resultingAcc.addScaledVector(forceAccum, inverseMass);

        // Update linear velocity from the acceleration.
        velocity.addScaledVector(resultingAcc, duration);

        // Impose drag.
        velocity *= real_pow(damping, duration);

        clearAccumulator();
    }

}