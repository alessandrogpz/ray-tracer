module;

export module rt.colors;

import rt.utils;

export namespace rt {

    struct Color {
        double r, g, b, a;

        Color() : r(1.0), g(1.0), b(1.0), a(1.0) {}
        Color(double _r, double _g, double _b, double _a = 1.0)
            : r(_r), g(_g), b(_b), a(_a) {}

        // Addition: a + b
        [[nodiscard]]
        Color operator+(const Color& other) const {
            return {r + other.r, g + other.g, b + other.b};
        }

        // Subtraction: a - b
        [[nodiscard]]
        Color operator-(const Color& other) const {
            return {r - other.r, g - other.g, b - other.b};
        }

        // Scalar Multiplication a * scalar
        [[nodiscard]]
        Color operator*(double scalar) const {
            return {r * scalar, g * scalar, b * scalar};
        }

        // Multiplying Colors
        [[nodiscard]]
        Color operator*(const Color &other) const {
            return {r * other.r, g * other.g, b * other.b};
        }

        [[nodiscard]]
        bool operator==(const Color& other) const {
            return equal(r, other.r) &&
                   equal(g, other.g) &&
                   equal(b, other.b);
        }
    };

    [[nodiscard]]
    Color createColor(double r, double g, double blue)
    {
        return {r, g, blue};
    }

} // namespace rt
