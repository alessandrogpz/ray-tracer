module rt.colors;

import rt.utils;

namespace rt {

    Color::Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color::Color(float _r, float _g, float _b, float _a)
        : r(_r), g(_g), b(_b), a(_a) {}

    Color Color::operator+(const Color& other) const {
        return {r + other.r, g + other.g, b + other.b};
    }

    Color Color::operator-(const Color& other) const {
        return {r - other.r, g - other.g, b - other.b};
    }

    Color Color::operator*(float scalar) const {
        return {r * scalar, g * scalar, b * scalar};
    }

    Color Color::operator*(const Color &other) const {
        return {r * other.r, g * other.g, b * other.b};
    }

    bool Color::operator==(const Color& other) const {
        return equal(r, other.r) &&
               equal(g, other.g) &&
               equal(b, other.b);
    }

    Color createColor(float r, float g, float blue) {
        return {r, g, blue};
    }

} // namespace rt
