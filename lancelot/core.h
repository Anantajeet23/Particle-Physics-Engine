#include "precision.h"
#include <cmath>

namespace lancelot {

    class Vector3d {
    private:
        real pad;

    public:
        real x; // x-coordinate of the vector
        real y; // y-coordinate of the vector
        real z; // z-coordinate of the vector

        // Constructors
        Vector3d();
        Vector3d(const real x, const real y, const real z);

        // Vector operations
        void invert();
        real magnitude() const;
        real squaredMagnitude() const;
        void normalize();

        // Scalar operations
        void operator*=(const real scalar);
        Vector3d operator*(const real scalar) const;

        // Vector addition and subtraction
        void operator+=(const Vector3d& v);
        Vector3d operator+(const Vector3d& v) const;
        void operator-=(const Vector3d& v);
        Vector3d operator-(const Vector3d& v) const;

        // Component-wise operations
        void addScaledVector(const Vector3d& v, real scalar);
        Vector3d componentProduct(const Vector3d& v) const;
        void componentProductUpdate(const Vector3d& v);

        // Dot and cross products
        real dotProduct(const Vector3d& v) const;
        real operator*(const Vector3d& v) const;
        Vector3d crossProduct(const Vector3d& v) const;
        void operator%=(const Vector3d& v);
        Vector3d operator%(const Vector3d& v) const;

        // Utility
        void clear();
    };

}