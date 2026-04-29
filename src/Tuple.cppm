module;

#include <cmath>
#include <cassert>
#include <stdexcept>

export module rt.tuple;
import rt.utils;

export namespace rt {

    struct tuple
    {
        float x, y, z, w;

        tuple() : x(0), y(0), z(0), w(0) {}
        tuple(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w) {}

        // Generic Tuple Scalar Overload Operations --------------------------------------
        // Scalar Multiplication: a * scalar
        [[nodiscard]] tuple operator*(float scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        // Scalar Division: a / scalar
        [[nodiscard]] tuple operator/(float scalar) const {
            assert(scalar != 0.0f && "Attempted to divide a tuple by zero");

            float scalar_inv = 1.0f / scalar;
            return {x * scalar_inv, y * scalar_inv, z * scalar_inv, w * scalar_inv};
        }

        // Getters & Setters -----------------------------------------------------------
        // Getter: float f = t[0];
        float operator[](size_t index) const {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
                default: throw std::out_of_range("Tuple index out of bounds");
            }
        }

        // Setter: t[0] = 5.0f;
        float& operator[](size_t index) {
            switch (index) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
                default: throw std::out_of_range("Tuple index out of bounds");
            }
        }

        // Overload for tuple comparison (==)
        bool operator==(const tuple& other) const {
            return 
                equal(x, other.x) &&
                equal(y, other.y) &&
                equal(z, other.z) &&
                equal(w, other.w);
        }

        // Overload for tuple comparison (!=)
        bool operator!=(const tuple& other) const {
            return !(*this == other);
        }
    };

    // Distinct Derived types inheriting from tuple -------------------------------
    struct point : public tuple {
        point(float x, float y, float z) : tuple(x, y, z, 1.0f) {}
        explicit point(const tuple& t) : tuple(t) { assert(equal(t.w, 1.0f)); }
    };

    struct vector : public tuple {
        vector(float x, float y, float z) : tuple(x, y, z, 0.0f) {}
        explicit vector(const tuple& t) : tuple(t) { assert(equal(t.w, 0.0f)); }
    };

    // Specific Geometry Overloads (Non-member functions) -------------------------
    // Point + Vector = Point
    [[nodiscard]] inline point operator+(const point& p, const vector& v) {
        return {p.x + v.x, p.y + v.y, p.z + v.z};
    }

    // Vector + Point = Point (Commutative property)
    [[nodiscard]] inline point operator+(const vector& v, const point& p) {
        return p + v; // Simply reuse the logic above
    }

    // Vector + Vector
    [[nodiscard]] inline vector operator+(const vector& v1, const vector& v2) {
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    // Point - Point
    [[nodiscard]] inline vector operator-(const point& a, const point& b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    // Point - Vector
    [[nodiscard]] inline point operator-(const point& p, const vector& v) {
        return {p.x - v.x, p.y - v.y, p.z - v.z};
    }

    // Vector - Vector
    [[nodiscard]] inline vector operator-(const vector& v1, const vector& v2) {
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    // Vector * -1 (Only Vectors can be Inverted)
    [[nodiscard]] inline vector operator-(const vector& v) {
        return {-v.x, -v.y, -v.z};
    }

    // Vector * Scalar
    [[nodiscard]] inline vector operator*(const vector& v, float scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    // Scalar * Vector (Commutative)
    [[nodiscard]] inline vector operator*(float scalar, const vector& v) {
        return v * scalar;
    }

    // Vector / Scalar
    [[nodiscard]] inline vector operator/(const vector& v, float scalar) {
        assert(scalar != 0.0f && "Attempted to divide a vector by zero");
        float inv = 1.0f / scalar;
        return {v.x * inv, v.y * inv, v.z * inv};
    }

    // Factory Utilities ---------------------------------------------------------
    [[nodiscard]] point createPoint(float x, float y, float z)
    {
        return {x, y, z};
    }

    [[nodiscard]] vector createVector(float x, float y, float z)
    {
        return {x, y, z};
    }

    // Operation Utilities ------------------------------------------------------
    [[nodiscard]] inline vector negateVector(vector a) {
        return -a;
    }

    [[nodiscard]] float getVectorMagnitude(vector a)
    {
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    [[nodiscard]] vector normalizeVector(vector a)
    {
        float inv_mag = 1.0f / getVectorMagnitude(a);
        return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
    }

    [[nodiscard]] float dotProduct(vector a, vector b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    [[nodiscard]] vector crossProduct(vector a, vector b)
    {
        return (
            createVector(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x));
    }

} // namespace rt
