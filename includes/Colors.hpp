#ifndef COLORS_HPP
#define COLORS_HPP

struct color
{
    float r, g, b;

    color() : r(0), g(0), b(0) {}
    color(float _r, float _g, float _b)
        : r(_r), g(_g), b(_b) {}

    // Addition: a + b
    color operator+(const color& other) const {
        return color(r + other.r, g + other.g, b + other.b);
    }

    // Subtraction: a - b
    color operator-(const color& other) const {
        return color(r - other.r, g - other.g, b - other.b);
    }

    // Scalar Multiplication a * scalar
    color operator*(float scalar) const {
        return color(r * scalar, g * scalar, b * scalar);
    }

    // Multiplying Colors
    color operator*(const color &other) const {
        return color(r * other.r, g * other.g, b * other.b);
    }
};

color createColor(float r, float g, float blue);

#endif