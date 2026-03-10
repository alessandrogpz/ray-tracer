#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <cmath>
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
};

struct color
{
    float r, g, b;

    color() : r(0), g(0), b(0) {}
    color(float _r, float _g, float _b)
        : r(_r), g(_g), b(_b) {}

    // Addition: a + b
    color operator+(const color& other) const {
        return color(r + other.r, g + other.g, b + other.b);
    }

    // Subtraction: a - b
    color operator-(const color& other) const {
        return color(r - other.r, g - other.g, b - other.b);
    }

    // Scalar Multiplication a * scalar
    color operator*(float scalar) const {
        return color(r * scalar, g * scalar, b * scalar);
    }

    // Multiplying Colors
    color operator*(const color& other) const {
        return color(r * other.r, g * other.g, b * other.b);
    }
};

tuple createPoint(float x, float y, float z);
tuple createVector(float x, float y, float z);
color createColor(float r, float g, float blue);

bool equal(float a, float b);

tuple addTuples(tuple a, tuple b);
tuple subtractTuples(tuple a, tuple b);
tuple negateTuple(tuple a);
tuple multiplyTupleByScalar(tuple a, float s);
tuple divideTupleByScalar(tuple a, float s);
float getVectorMagnitude(tuple a);
tuple normalizeVector(tuple a);
float dotProduct(tuple a, tuple b);
tuple crossProduct(tuple a, tuple b);

#endif