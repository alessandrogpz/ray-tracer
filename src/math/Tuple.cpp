module;

#include <cassert>
#include <cmath>

module rt.tuple;

import std;

import rt.utils;

namespace rt {

    // --- Tuple Struct Implementations ---
    Tuple::Tuple() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Tuple::Tuple(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}

    Tuple Tuple::operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Tuple Tuple::operator/(float scalar) const {
        assert(scalar != 0.0f && "Attempted to divide a Tuple by zero");
        float scalar_inv = 1.0f / scalar;
        return {x * scalar_inv, y * scalar_inv, z * scalar_inv, w * scalar_inv};
    }

    float Tuple::operator[](std::size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Tuple index out of bounds");
        }
    }

    float& Tuple::operator[](std::size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Tuple index out of bounds");
        }
    }

    bool Tuple::operator==(const Tuple& other) const {
        return equal(x, other.x) &&
               equal(y, other.y) &&
               equal(z, other.z) &&
               equal(w, other.w);
    }

    bool Tuple::operator!=(const Tuple& other) const {
        return !(*this == other);
    }

    // --- Derived Types ---
    Point::Point() : Tuple(0.0f, 0.0f, 0.0f, 1.0f) {}
    Point::Point(float x, float y, float z) : Tuple(x, y, z, 1.0f) {}
    Point::Point(const Tuple& t) : Tuple(t) {
        assert(equal(t.w, 1.0f));
    }

    Vector::Vector() : Tuple(0.0f, 0.0f, 0.0f, 0.0f) {}
    Vector::Vector(float x, float y, float z) : Tuple(x, y, z, 0.0f) {}
    Vector::Vector(const Tuple& t) : Tuple(t) {
        assert(equal(t.w, 0.0f));
    }

    // --- Geometry Overloads ---
    Point operator+(const Point& p, const Vector& v) {
        return {p.x + v.x, p.y + v.y, p.z + v.z};
    }

    Point operator+(const Vector& v, const Point& p) {
        return p + v;
    }

    Vector operator+(const Vector& v1, const Vector& v2) {
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    Vector operator-(const Point& a, const Point& b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    Point operator-(const Point& p, const Vector& v) {
        return {p.x - v.x, p.y - v.y, p.z - v.z};
    }

    Vector operator-(const Vector& v1, const Vector& v2) {
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    Vector operator-(const Vector& v) {
        return {-v.x, -v.y, -v.z};
    }

    Vector operator*(const Vector& v, float scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    Vector operator*(float scalar, const Vector& v) {
        return v * scalar;
    }

    Vector operator/(const Vector& v, float scalar) {
        assert(scalar != 0.0f && "Attempted to divide a Vector by zero");
        float inv = 1.0f / scalar;
        return {v.x * inv, v.y * inv, v.z * inv};
    }

    // --- Factory Utilities ---
    Point createPoint(float x, float y, float z) {
        return {x, y, z};
    }

    Vector createVector(float x, float y, float z) {
        return {x, y, z};
    }

    // --- Operation Utilities ---
    Vector negateVector(const Vector& a) {
        return -a;
    }

    float getVectorMagnitude(const Vector& a) {
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    Vector normalizeVector(const Vector& a) {
        float inv_mag = 1.0f / getVectorMagnitude(a);
        return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
    }

    float dotProduct(const Vector& a, const Vector& b) {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    // Pass crossProduct parameters by const reference
    Vector crossProduct(const Vector& a, const Vector& b) {
        return createVector(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    Vector reflect(const Vector& in, const Vector& normal) {
        return in - normal * 2.0f * dotProduct(in, normal);
    }

} // namespace rt
