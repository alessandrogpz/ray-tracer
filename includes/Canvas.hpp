#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include "../includes/Colors.hpp"

struct canvas {
    int width;
    int height;
    std::vector<color> pixels;

    // 2D representation in 1D array
    // index = (y * width) + x

    canvas(int w,  int h) : width(w), height(h) {
        //Inititalize the verctor with w * h black colors
        pixels.resize(w * h, color(0, 0, 0));
    }
};

void writePixel(canvas &c, int x, int y, color col);
color pixelAt(const canvas &c, int x, int y);

#endif