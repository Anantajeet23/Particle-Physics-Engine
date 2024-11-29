#include "pfgen.h"
#include <vector>
#include <queue>

namespace lancelot {

    class Particle;

    class ParticleContact {
        friend class ParticleContactsResolver;

    public:
        Particle* particle1;
        Particle* particle2;
        real restitution;
        Vector3d contactNormal;
        real penetration;
        Vector3d particle1Movement;
        Vector3d particle2Movement;
        bool isResolved;
        real precomputedSeparatingVelocity;
        bool isP2Immovable;

        ParticleContact(Particle* particle1, Particle* particle2, real restitution, real penetration);
        ParticleContact(Particle* particle1, Particle* particle2, real restitution, real penetration, Vector3d contactNormal);

    protected:
        void resolve(real duration);
        real calculateSeparatingVelocity() const;

    private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);
    };

    class ParticleContactDetector {
    protected:
        std::vector<Particle*>* ParticleRegistry;
        std::vector<ParticleContact*> ParticleContactRegistry;
        unsigned maxContacts;
        real cubeSize;
        Vector3d cubeCenter;

    public:
        ParticleContactDetector();
        ParticleContactDetector(std::vector<Particle*>* pr, unsigned maxContacts, real cubeSize, Vector3d cubeCenter);

        void setCubeBoundary(real size, Vector3d center);
        std::vector<ParticleContact*>& getContactRegistry();
        unsigned GenerateContacts();
        void GenerateWallContacts();
    };

    class ParticleContactsResolver {
    protected:
        unsigned iterations;
        unsigned iterationsUsed;

    public:
        ParticleContactsResolver();
        ParticleContactsResolver(unsigned iterations);

        void setIterations(unsigned num);
        unsigned getIterations() const;
        void resolveContacts(std::vector<ParticleContact*> contactArray, real duration);

        struct ContactComparator {
            bool operator()(ParticleContact* a, ParticleContact* b);
        };
    };

}