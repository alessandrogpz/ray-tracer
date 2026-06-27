export module rt.tuple;

import std;

import rt.utils;

export namespace rt {

    struct Tuple
    {
        double x, y, z, w;

        Tuple();
        Tuple(double _x, double _y, double _z, double _w);

        // Generic Tuple Scalar Overload Operations
        [[nodiscard]] Tuple operator*(double scalar) const;
        [[nodiscard]] Tuple operator/(double scalar) const;

        // Getters & Setters
        [[nodiscard]] double operator[](std::size_t index) const;
        double& operator[](std::size_t index);

        // Overloads for Tuple comparison
        [[nodiscard]] bool operator==(const Tuple& other) const;
        [[nodiscard]] bool operator!=(const Tuple& other) const;
    };

    // Distinct Derived types inheriting from Tuple
    struct Point : public Tuple {
        Point();
        Point(double x, double y, double z);
        explicit Point(const Tuple& t);
    };

    struct Vector : public Tuple {
        Vector();
        Vector(double x, double y, double z);
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
    [[nodiscard]] Vector operator*(const Vector& v, double scalar);
    [[nodiscard]] Vector operator*(double scalar, const Vector& v);
    [[nodiscard]] Vector operator/(const Vector& v, double scalar);

    // Factory Utilities
    [[nodiscard]] Point createPoint(double x, double y, double z);
    [[nodiscard]] Vector createVector(double x, double y, double z);

    // Operation Utilities
    [[nodiscard]] Vector negateVector(const Vector& a);
    [[nodiscard]] double getVectorMagnitude(const Vector& a);
    [[nodiscard]] Vector normalizeVector(const Vector& a);
    [[nodiscard]] double dotProduct(const Vector& a, const Vector& b);
    [[nodiscard]] Vector crossProduct(const Vector& a, const Vector& b);
    [[nodiscard]] Vector reflect(const Vector& in, const Vector& normal);

} // namespace rt
