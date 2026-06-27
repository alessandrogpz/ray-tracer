export module rt.tuple;

import std;

import rt.utils;

export namespace rt {

    struct Tuple
    {
        float x, y, z, w;

        Tuple();
        Tuple(float _x, float _y, float _z, float _w);

        // Generic Tuple Scalar Overload Operations
        [[nodiscard]] Tuple operator*(float scalar) const;
        [[nodiscard]] Tuple operator/(float scalar) const;

        // Getters & Setters
        [[nodiscard]] float operator[](std::size_t index) const;
        float& operator[](std::size_t index);

        // Overloads for Tuple comparison
        [[nodiscard]] bool operator==(const Tuple& other) const;
        [[nodiscard]] bool operator!=(const Tuple& other) const;
    };

    // Distinct Derived types inheriting from Tuple
    struct Point : public Tuple {
        Point();
        Point(float x, float y, float z);
        explicit Point(const Tuple& t);
    };

    struct Vector : public Tuple {
        Vector();
        Vector(float x, float y, float z);
        explicit Vector(const Tuple& t);
    };

    // Specific Geometry Overloads (Non-member functions)
    [[nodiscard]] Point operator+(const Point& p, const Vector& v);
    [[nodiscard]] Point operator+(const Vector& v, const Point& p);
    [[nodiscard]] Vector operator+(const Vector& v1, const Vector& v2);
    [[nodiscard]] Vector operator-(const Point& a, const Point& b);
    [[nodiscard]] Point operator-(const Point& p, const Vector& v);
    [[nodiscard]] Vector operator-(const Vector& v1, const Vector& v2);
    [[nodiscard]] Vector operator-(const Vector& v);
    [[nodiscard]] Vector operator*(const Vector& v, float scalar);
    [[nodiscard]] Vector operator*(float scalar, const Vector& v);
    [[nodiscard]] Vector operator/(const Vector& v, float scalar);

    // Factory Utilities
    [[nodiscard]] Point createPoint(float x, float y, float z);
    [[nodiscard]] Vector createVector(float x, float y, float z);

    // Operation Utilities
    [[nodiscard]] Vector negateVector(const Vector& a);
    [[nodiscard]] float getVectorMagnitude(const Vector& a);
    [[nodiscard]] Vector normalizeVector(const Vector& a);
    [[nodiscard]] float dotProduct(const Vector& a, const Vector& b);
    [[nodiscard]] Vector crossProduct(const Vector& a, const Vector& b);
    [[nodiscard]] Vector reflect(const Vector& in, const Vector& normal);

} // namespace rt
