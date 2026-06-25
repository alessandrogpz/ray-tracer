module rt.colors;

import rt.utils;

namespace rt {

    Color::Color() : r(1.0), g(1.0), b(1.0), a(1.0) {}
    Color::Color(double _r, double _g, double _b, double _a)
        : r(_r), g(_g), b(_b), a(_a) {}

    Color Color::operator+(const Color& other) const {
        return {r + other.r, g + other.g, b + other.b};
    }

    Color Color::operator-(const Color& other) const {
        return {r - other.r, g - other.g, b - other.b};
    }

    Color Color::operator*(double scalar) const {
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

    Color createColor(double r, double g, double blue) {
        return {r, g, blue};
    }

} // namespace rt
