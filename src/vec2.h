#ifndef VEC2_H
#define VEC2_H

#include <cmath>

struct Vec2 {
    double x;
    double y;

    Vec2() : x(0), y(0) {}
    Vec2(double x, double y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(double scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator/(double scalar) const {
        return Vec2(x / scalar, y / scalar);
    }
    
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    double length2() const {
        return x * x + y * y;
    }

    double length() const {
        return std::sqrt(length2());
    }

    Vec2 normalized() const {
        double len = length();
        if (len > 0) { // ToDo: use epsilon instead for stabiltiy
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }
};

#endif // VEC2_H