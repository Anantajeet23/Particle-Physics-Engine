#include "precision.h"
#include "core.h"
#include <cmath>

namespace lancelot {

    // Default constructor
    Vector3d::Vector3d() {
        x = 0;
        y = 0;
        z = 0;
    }

    // Parameterized constructor
    Vector3d::Vector3d(const real x, const real y, const real z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Method to invert the vector
    void Vector3d::invert() {
        x = -x;
        y = -y;
        z = -z;
    }

    // Method to return magnitude
    real Vector3d::magnitude() const {
        return real_sqrt(x * x + y * y + z * z);
    }

    // Method to return squared magnitude
    real Vector3d::squaredMagnitude() const {
        return x * x + y * y + z * z;
    }

    // Method to normalize the vector
    void Vector3d::normalize() {
        real d = magnitude();
        if (d > 0) {
            x /= d;
            y /= d;
            z /= d;
        }
    }

    // Scalar multiplication
    void Vector3d::operator*=(const real scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    Vector3d Vector3d::operator*(const real scalar) const {
        return Vector3d(x * scalar, y * scalar, z * scalar);
    }

    // Vector addition
    void Vector3d::operator+=(const Vector3d& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    Vector3d Vector3d::operator+(const Vector3d& v) const {
        return Vector3d(x + v.x, y + v.y, z + v.z);
    }

    // Vector subtraction
    void Vector3d::operator-=(const Vector3d& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    Vector3d Vector3d::operator-(const Vector3d& v) const {
        return Vector3d(x - v.x, y - v.y, z - v.z);
    }

    // Add scaled vector
    void Vector3d::addScaledVector(const Vector3d& v, real scalar) {
        x += v.x * scalar;
        y += v.y * scalar;
        z += v.z * scalar;
    }

    // Component-wise operations
    Vector3d Vector3d::componentProduct(const Vector3d& v) const {
        return Vector3d(x * v.x, y * v.y, z * v.z);
    }

    void Vector3d::componentProductUpdate(const Vector3d& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    // Dot product
    real Vector3d::dotProduct(const Vector3d& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    real Vector3d::operator*(const Vector3d& v) const {
        return dotProduct(v);
    }

    // Cross product
    Vector3d Vector3d::crossProduct(const Vector3d& v) const {
        return Vector3d(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    void Vector3d::operator%=(const Vector3d& v) {
        *this = crossProduct(v);
    }

    Vector3d Vector3d::operator%(const Vector3d& v) const {
        return crossProduct(v);
    }

    // Zero all components
    void Vector3d::clear() {
        x = y = z = 0;
    }

}