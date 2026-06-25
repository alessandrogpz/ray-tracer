module;

export module rt.colors;

import rt.utils;

export namespace rt {

    struct color
    {
        double r, g, b;

        color() : r(0), g(0), b(0) {}
        color(double _r, double _g, double _b)
            : r(_r), g(_g), b(_b) {}

        // Addition: a + b
        [[nodiscard]]
        color operator+(const color& other) const {
            return {r + other.r, g + other.g, b + other.b};
        }

        // Subtraction: a - b
        [[nodiscard]]
        color operator-(const color& other) const {
            return {r - other.r, g - other.g, b - other.b};
        }

        // Scalar Multiplication a * scalar
        [[nodiscard]]
        color operator*(double scalar) const {
            return {r * scalar, g * scalar, b * scalar};
        }

        // Multiplying Colors
        [[nodiscard]]
        color operator*(const color &other) const {
            return {r * other.r, g * other.g, b * other.b};
        }

        [[nodiscard]]
        bool operator==(const color& other) const {
            return equal(r, other.r) &&
                   equal(g, other.g) &&
                   equal(b, other.b);
        }
    };

    [[nodiscard]]
    color createColor(double r, double g, double blue)
    {
        return {r, g, blue};
    }

} // namespace rt
