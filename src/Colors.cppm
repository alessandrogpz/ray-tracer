module;

export module rt.colors;

export namespace rt {

    struct color
    {
        float r, g, b;

        color() : r(0), g(0), b(0) {}
        color(float _r, float _g, float _b)
            : r(_r), g(_g), b(_b) {}

        // Addition: a + b
        [[nodiscard]] color operator+(const color& other) const {
            return {r + other.r, g + other.g, b + other.b};
        }

        // Subtraction: a - b
        [[nodiscard]] color operator-(const color& other) const {
            return {r - other.r, g - other.g, b - other.b};
        }

        // Scalar Multiplication a * scalar
        [[nodiscard]] color operator*(float scalar) const {
            return {r * scalar, g * scalar, b * scalar};
        }

        // Multiplying Colors
        [[nodiscard]] color operator*(const color &other) const {
            return {r * other.r, g * other.g, b * other.b};
        }
    };

    [[nodiscard]] color createColor(float r, float g, float blue)
    {
        return {r, g, blue};
    }

} // namespace rt
