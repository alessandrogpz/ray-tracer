export module rt.colors;

import rt.utils;

export namespace rt {

    struct Color {
        float r, g, b, a;

        Color();
        Color(float _r, float _g, float _b, float _a = 1.0f);

        // Addition: a + b
        [[nodiscard]] Color operator+(const Color& other) const;

        // Subtraction: a - b
        [[nodiscard]] Color operator-(const Color& other) const;

        // Scalar Multiplication a * scalar
        [[nodiscard]] Color operator*(float scalar) const;

        // Multiplying Colors
        [[nodiscard]] Color operator*(const Color &other) const;

        [[nodiscard]] bool operator==(const Color& other) const;
    };

    [[nodiscard]] Color createColor(float r, float g, float blue);

} // namespace rt
