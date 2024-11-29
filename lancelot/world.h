#include "pcontacts.h"
#include <vector>
#include <iostream>

namespace lancelot {

    class World {
    public:
        /* List of all the particles in the world */
        std::vector<Particle*> ParticleRegistry;

        /* Object to store all the particle and force generator pairs */
        ParticleForceRegistry PForceRegistry;

        /* Detector to find contacts */
        ParticleContactDetector Detector;

        /* Resolver to resolve the contacts */
        ParticleContactsResolver Resolver;

        /* Constructor */
        World(unsigned maxContacts, unsigned iterations = 0);

        // Remove all forces from the accumulator.
        void startFrame();

        // Integrates all particles forward in time by the given duration.
        void integrate(real duration);

        // Prints information about the contacts (debugging purposes).
        int printStuff();

        // Simulates one step of the physics simulation.
        void simulatePhysics(real duration);
    };

}