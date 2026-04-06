#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <cassert>

struct tuple
{
    float x, y, z, w;

    tuple() : x(0), y(0), z(0), w(0) {}
    tuple(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}

    // Addition: a + b
    tuple operator+(const tuple& other) const {
        return tuple(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Subtraction: a - b
    tuple operator-(const tuple& other) const {
        return tuple(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Scalar Multiplication: a * scalar
    tuple operator*(float scalar) const {
        return tuple(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Scalar Division: a / scalar
    tuple operator/(float scalar) const {
        assert(scalar != 0.0f && "Attempted to divide a tuple by zero");

	    float scalar_inv = 1.0f / scalar;
        return tuple(x * scalar_inv, y * scalar_inv, z * scalar_inv, w * scalar_inv);
    }
};

tuple createPoint(float x, float y, float z);
tuple createVector(float x, float y, float z);

tuple negateTuple(tuple a);
float getVectorMagnitude(tuple a);
tuple normalizeVector(tuple a);
float dotProduct(tuple a, tuple b);
tuple crossProduct(tuple a, tuple b);

#endif