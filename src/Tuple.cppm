module;

#include <cmath>
#include <cassert>
#include <stdexcept>

export module rt.tuple;
import rt.utils;

export namespace rt {

    struct Tuple
    {
        double x, y, z, w;

        Tuple() : x(0), y(0), z(0), w(0) {}
        Tuple(double _x, double _y, double _z, double _w)
            : x(_x), y(_y), z(_z), w(_w) {}

        // Generic Tuple Scalar Overload Operations --------------------------------------
        // Scalar Multiplication: a * scalar
        [[nodiscard]]
        Tuple operator*(double scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        // Scalar Division: a / scalar
        [[nodiscard]]
        Tuple operator/(double scalar) const {
            assert(scalar != 0.0 && "Attempted to divide a Tuple by zero");

            double scalar_inv = 1.0 / scalar;
            return {x * scalar_inv, y * scalar_inv, z * scalar_inv, w * scalar_inv};
        }

        // Getters & Setters -----------------------------------------------------------
        // Getter: double f = t[0];
        double operator[](size_t index) const {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
                default: throw std::out_of_range("Tuple index out of bounds");
            }
        }

        // Setter: t[0] = 5.0;
        double& operator[](size_t index) {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
                default: throw std::out_of_range("Tuple index out of bounds");
            }
        }

        // Overload for Tuple comparison (==)
        bool operator==(const Tuple& other) const {
            return 
                equal(x, other.x) &&
                equal(y, other.y) &&
                equal(z, other.z) &&
                equal(w, other.w);
        }

        // Overload for Tuple comparison (!=)
        bool operator!=(const Tuple& other) const {
            return !(*this == other);
        }
    };

    // Distinct Derived types inheriting from Tuple -------------------------------
    struct Point : public Tuple {
        Point() : Tuple(0.0, 0.0, 0.0, 1.0) {}
        Point(double x, double y, double z) : Tuple(x, y, z, 1.0) {}
        explicit Point(const Tuple& t) : Tuple(t) { assert(equal(t.w, 1.0)); }
    };

    struct Vector : public Tuple {
        Vector() : Tuple (0.0, 0.0, 0.0, 0.0) {}
        Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}
        explicit Vector(const Tuple& t) : Tuple(t) { assert(equal(t.w, 0.0)); }
    };

    // Specific Geometry Overloads (Non-member functions) -------------------------
    // Point + Vector = Point
    [[nodiscard]]
    inline Point operator+(Point p, Vector v) {
        return {p.x + v.x, p.y + v.y, p.z + v.z};
    }

    // Vector + Point = Point (Commutative property)
    [[nodiscard]]
    inline Point operator+(Vector v, Point p) {
        return p + v;
    }

    // Vector + Vector
    [[nodiscard]]
    inline Vector operator+(Vector v1, Vector v2) {
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    // Point - Point
    [[nodiscard]]
    inline Vector operator-(Point a, Point b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    // Point - Vector
    [[nodiscard]]
    inline Point operator-(Point p, Vector v) {
        return {p.x - v.x, p.y - v.y, p.z - v.z};
    }

    // Vector - Vector
    [[nodiscard]]
    inline Vector operator-(Vector v1, Vector v2) {
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    // Vector * -1 (Only Vectors can be Inverted)
    [[nodiscard]]
    inline Vector operator-(Vector v) {
        return {-v.x, -v.y, -v.z};
    }

    // Vector * Scalar
    [[nodiscard]]
    inline Vector operator*(Vector v, double scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    // Scalar * Vector (Commutative)
    [[nodiscard]]
    inline Vector operator*(double scalar, Vector v) {
        return v * scalar;
    }

    // Vector / Scalar
    [[nodiscard]]
    inline Vector operator/(Vector v, double scalar) {
        assert(scalar != 0.0 && "Attempted to divide a Vector by zero");
        double inv = 1.0 / scalar;
        return {v.x * inv, v.y * inv, v.z * inv};
    }

    // Factory Utilities ---------------------------------------------------------
    [[nodiscard]]
    Point createPoint(double x, double y, double z)
    {
        return {x, y, z};
    }

    [[nodiscard]]
    Vector createVector(double x, double y, double z)
    {
        return {x, y, z};
    }

    // Operation Utilities ------------------------------------------------------
    [[nodiscard]]
    inline Vector negateVector(Vector a) {
        return -a;
    }

    [[nodiscard]]
    double getVectorMagnitude(Vector a)
    {
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    [[nodiscard]]
    Vector normalizeVector(Vector a)
    {
        double inv_mag = 1.0 / getVectorMagnitude(a);
        return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
    }

    [[nodiscard]]
    double dotProduct(Vector a, Vector b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    [[nodiscard]]
    Vector crossProduct(Vector a, Vector b)
    {
        return (
            createVector(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x));
    }

    [[nodiscard]]
    Vector reflect(Vector in, Vector normal)
    {
        return in - normal * 2.0 * dotProduct(in, normal);
    }
} // namespace rt
