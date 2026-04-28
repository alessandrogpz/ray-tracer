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

        // Addition: a + b
        [[nodiscard]] tuple operator+(const tuple& other) const {
            return {x + other.x, y + other.y, z + other.z, w + other.w};
        }

        // Subtraction: a - b
        [[nodiscard]] tuple operator-(const tuple& other) const {
            return {x - other.x, y - other.y, z - other.z, w - other.w};
        }

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

    [[nodiscard]] tuple createPoint(float x, float y, float z)
    {
        return {x, y, z, 1.0f};
    }

    [[nodiscard]] tuple createVector(float x, float y, float z)
    {
        return {x, y, z, 0.0f};
    }

    [[nodiscard]] tuple negateTuple(tuple a)
    {
        return tuple(a * -1);
    }

    [[nodiscard]] float getVectorMagnitude(tuple a)
    {
        assert(equal(a.w, 0.0f) && "Argument 'a' to getVectorMagnitude must be a vector");
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    [[nodiscard]] tuple normalizeVector(tuple a)
    {
        assert(equal(a.w, 0.0f) && "Argument 'a' to normalizeVector must be a vector");
        float inv_mag = 1.0f / getVectorMagnitude(a);
        return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
    }

    [[nodiscard]] float dotProduct(tuple a, tuple b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    }

    [[nodiscard]] tuple crossProduct(tuple a, tuple b)
    {
        assert(equal(a.w, 0.0f) && "Argument 'a' to crossProduct must be a vector");
        assert(equal(b.w, 0.0f) && "Argument 'b' to crossProduct must be a vector");

        return (
            createVector(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x));
    }

} // namespace rt
