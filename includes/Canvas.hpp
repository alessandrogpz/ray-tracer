#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "Colors.hpp"

#include <vector>
#include <string>

struct canvas {
    size_t width;
    size_t height;
    color initialColor;
    std::vector<color> pixels;

    // 2D representation in 1D array
    // index = (y * width) + x

    canvas(size_t w,  size_t h) : width(w), height(h) {
        //Inititalize the verctor with w * h black colors
        pixels.resize(w * h, color(0, 0, 0));
    }

    canvas(size_t w, size_t h, color c) : width(w), height(h), initialColor(c) {
        //Inititalize the verctor with w * h with initialColor
        pixels.resize(w * h, initialColor);
    }
};

void writePixel(canvas &c, size_t x, size_t y, color col);
[[nodiscard]] color pixelAt(const canvas &c, size_t x, size_t y);
[[nodiscard]] std::string canvasToPPM(const canvas &c);
void savePPM(const std::string& filename, const std::string& ppmData);

#endif