#include "particle.h"
#include <vector>

namespace lancelot {

    class ParticleForceGenerator {
    public:
        /**
         * Overload this in implementations of the interface to calculate
         * and update the force applied to the given particle.
         */
        virtual void update(lancelot::Particle* particle, lancelot::real duration) = 0;
    };

    class ParticleForceRegistry {
    protected:
        /* Structure used to hold every particle and the force generators pair */
        struct ParticleForce {
            Particle* Particle;
            ParticleForceGenerator* fg;
        };

        typedef std::vector<ParticleForce> Registry;

        Registry registrations;

    public:
        void add(Particle* particle, ParticleForceGenerator* fg);
        //void remove(Particle* particle, ParticleForceGenerator* fg);
        void clear();
        void updateForces(real duration);
    };

    class ParticleGravity : public ParticleForceGenerator {
    public:
        lancelot::Vector3d gravity;

        ParticleGravity(const Vector3d& gravity);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

    class ParticlePairwiseGravity : public ParticleForceGenerator {
    public:
        real G;
        std::vector<Particle*>* particles;

        ParticlePairwiseGravity(real G, std::vector<Particle*>* particles);
        void update(Particle* particle, real duration);
    };

    class ParticleCOMGravity : public ParticleForceGenerator {
    public:
        real G;
        std::vector<Particle*>* particles;
        Vector3d center;

        ParticleCOMGravity(real G, std::vector<Particle*>* particles, const Vector3d& center);
        void update(Particle* particle, real duration);
    };

    class ParticleDrag : public ParticleForceGenerator {
    public:
        real k1;
        real k2;

        ParticleDrag(real k1, real k2);
        void update(Particle* particle, real duration);
    };

    class ParticleString : public ParticleForceGenerator {
    public:
        Particle* other;
        real springConstant;
        real restLength;

        ParticleString(Particle* other, real springConst, real restLength);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

    class ParticleAnchoredString : public ParticleForceGenerator {
    public:
        Vector3d* anchor;
        real springConstant;
        real restLength;

        ParticleAnchoredString(Vector3d* anchor, real springConst, real restLength);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

    class ParticleBungee : public ParticleForceGenerator {
    public:
        Particle* other;
        real springConstant;
        real restLength;

        ParticleBungee(Particle* other, real springConst, real restLength);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

    class ParticleAnchoredBungee : public ParticleForceGenerator {
    public:
        Vector3d* anchor;
        real springConstant;
        real restLength;

        ParticleAnchoredBungee(Vector3d* anchor, real springConst, real restLength);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

    class ParticleBuoyancy : public ParticleForceGenerator {
    public:
        real maxDepth;
        real volume;
        real liquidHeight;
        real liquidDensity;

        ParticleBuoyancy(real maxDepth, real volume, real liquidHeight, real liquidDensity);
        void update(lancelot::Particle* particle, lancelot::real duration);
    };

}