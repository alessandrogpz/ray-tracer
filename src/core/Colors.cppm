export module rt.colors;

import rt.utils;

export namespace rt {

    struct Color {
        double r, g, b, a;

        Color();
        Color(double _r, double _g, double _b, double _a = 1.0);

        // Addition: a + b
        [[nodiscard]] Color operator+(const Color& other) const;

        // Subtraction: a - b
        [[nodiscard]] Color operator-(const Color& other) const;

        // Scalar Multiplication a * scalar
        [[nodiscard]] Color operator*(double scalar) const;

        // Multiplying Colors
        [[nodiscard]] Color operator*(const Color &other) const;

        [[nodiscard]] bool operator==(const Color& other) const;
    };

    [[nodiscard]] Color createColor(double r, double g, double blue);

} // namespace rt
