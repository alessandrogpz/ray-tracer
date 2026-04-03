#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <string>
#include "../includes/Colors.hpp"

struct canvas {
    int width;
    int height;
    color initialColor;
    std::vector<color> pixels;

    // 2D representation in 1D array
    // index = (y * width) + x

    canvas(int w,  int h) : width(w), height(h) {
        //Inititalize the verctor with w * h black colors
        pixels.resize(w * h, color(0, 0, 0));
    }

    canvas(int w, int h, color c) : width(w), height(h), initialColor(c) {
        //Inititalize the verctor with w * h with initialColor
        pixels.resize(w * h, initialColor);
    }
};

void writePixel(canvas &c, int x, int y, color col);
color pixelAt(const canvas &c, int x, int y);
std::string canvasToPPM(const canvas &c);
void savePPM(const std::string& filename, const std::string& ppmData);

#endif