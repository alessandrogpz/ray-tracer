module;

#include <cstddef>

export module rt.tuple;
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
        [[nodiscard]] double operator[](size_t index) const;
        double& operator[](size_t index);

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
    [[nodiscard]] Point operator+(Point p, Vector v);
    [[nodiscard]] Point operator+(Vector v, Point p);
    [[nodiscard]] Vector operator+(Vector v1, Vector v2);
    [[nodiscard]] Vector operator-(Point a, Point b);
    [[nodiscard]] Point operator-(Point p, Vector v);
    [[nodiscard]] Vector operator-(Vector v1, Vector v2);
    [[nodiscard]] Vector operator-(Vector v);
    [[nodiscard]] Vector operator*(Vector v, double scalar);
    [[nodiscard]] Vector operator*(double scalar, Vector v);
    [[nodiscard]] Vector operator/(Vector v, double scalar);

    // Factory Utilities
    [[nodiscard]] Point createPoint(double x, double y, double z);
    [[nodiscard]] Vector createVector(double x, double y, double z);

    // Operation Utilities
    [[nodiscard]] Vector negateVector(Vector a);
    [[nodiscard]] double getVectorMagnitude(Vector a);
    [[nodiscard]] Vector normalizeVector(Vector a);
    [[nodiscard]] double dotProduct(Vector a, Vector b);
    [[nodiscard]] Vector crossProduct(Vector a, Vector b);
    [[nodiscard]] Vector reflect(Vector in, Vector normal);

} // namespace rt
