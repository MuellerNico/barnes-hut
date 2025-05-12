#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct Vec3 {
    double x;
    double y;
    double z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(double scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(double scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
    
    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    double length2() const {
        return x * x + y * y + z * z;
    }

    double length() const {
        return std::sqrt(length2());
    }

    Vec3 normalized() const {
        double len = length();
        if (len == 0) return Vec3(0, 0, 0); // avoid division by zero
        return Vec3(x / len, y / len, z / len);
    }

    double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

#endif // VEC3_H