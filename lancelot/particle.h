#include "core.h"
#include <cassert>

namespace lancelot {

    class Particle {
    private:
        real Mass;               // Mass of the particle
        real inverseMass;        // Inverse of the particle's mass
        real radius;             // Radius of the particle
        Vector3d forceAccum;     // Accumulated force for the next iteration
        Vector3d position;       // Position vector of the particle
        Vector3d velocity;       // Velocity vector of the particle
        Vector3d acceleration;   // Acceleration vector of the particle
        real damping;            // Linear damping factor

    public:
        // Getters
        real getMass() const;
        real getInverseMass() const;
        real getRadius() const;
        Vector3d getForceAccum() const;
        Vector3d getPosition() const;
        Vector3d getVelocity() const;
        Vector3d getAcceleration() const;
        real getDamping() const;

        // Setters
        void setMass(real m);
        void setRadius(real r);
        void setPosition(const Vector3d& new_position);
        void setVelocity(const Vector3d& new_velocity);
        void setAcceleration(const Vector3d& new_acceleration);
        void setDamping(real damp);

        // Utility functions
        void clearAccumulator();                   // Clears the accumulated force
        void addForce(const Vector3d& force);      // Adds force for the next iteration
        void integrate(real duration);             // Updates the particle's position and velocity
    };

}