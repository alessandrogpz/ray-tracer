module;

#include <cassert>
#include <cmath>

module rt.tuple;

import std;

import rt.utils;

namespace rt {

    // --- Tuple Struct Implementations ---
    Tuple::Tuple() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    Tuple::Tuple(double _x, double _y, double _z, double _w)
        : x(_x), y(_y), z(_z), w(_w) {}

    Tuple Tuple::operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Tuple Tuple::operator/(double scalar) const {
        assert(scalar != 0.0 && "Attempted to divide a Tuple by zero");
        double scalar_inv = 1.0 / scalar;
        return {x * scalar_inv, y * scalar_inv, z * scalar_inv, w * scalar_inv};
    }

    double Tuple::operator[](std::size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Tuple index out of bounds");
        }
    }

    double& Tuple::operator[](std::size_t index) {
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
    Point::Point() : Tuple(0.0, 0.0, 0.0, 1.0) {}
    Point::Point(double x, double y, double z) : Tuple(x, y, z, 1.0) {}
    Point::Point(const Tuple& t) : Tuple(t) {
        assert(equal(t.w, 1.0));
    }

    Vector::Vector() : Tuple(0.0, 0.0, 0.0, 0.0) {}
    Vector::Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}
    Vector::Vector(const Tuple& t) : Tuple(t) {
        assert(equal(t.w, 0.0));
    }

    // --- Geometry Overloads ---
    Point operator+(Point p, Vector v) {
        return {p.x + v.x, p.y + v.y, p.z + v.z};
    }

    Point operator+(Vector v, Point p) {
        return p + v;
    }

    Vector operator+(Vector v1, Vector v2) {
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    Vector operator-(Point a, Point b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    Point operator-(Point p, Vector v) {
        return {p.x - v.x, p.y - v.y, p.z - v.z};
    }

    Vector operator-(Vector v1, Vector v2) {
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    Vector operator-(Vector v) {
        return {-v.x, -v.y, -v.z};
    }

    Vector operator*(Vector v, double scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    Vector operator*(double scalar, Vector v) {
        return v * scalar;
    }

    Vector operator/(Vector v, double scalar) {
        assert(scalar != 0.0 && "Attempted to divide a Vector by zero");
        double inv = 1.0 / scalar;
        return {v.x * inv, v.y * inv, v.z * inv};
    }

    // --- Factory Utilities ---
    Point createPoint(double x, double y, double z) {
        return {x, y, z};
    }

    Vector createVector(double x, double y, double z) {
        return {x, y, z};
    }

    // --- Operation Utilities ---
    Vector negateVector(Vector a) {
        return -a;
    }

    double getVectorMagnitude(Vector a) {
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    Vector normalizeVector(Vector a) {
        double inv_mag = 1.0 / getVectorMagnitude(a);
        return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
    }

    double dotProduct(Vector a, Vector b) {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    Vector crossProduct(Vector a, Vector b) {
        return createVector(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    Vector reflect(Vector in, Vector normal) {
        return in - normal * 2.0 * dotProduct(in, normal);
    }

} // namespace rt
