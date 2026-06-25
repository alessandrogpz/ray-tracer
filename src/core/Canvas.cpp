module;

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <format>

module rt.canvas;

import rt.colors;

namespace rt {

    Canvas::Canvas(size_t w, size_t h) : width(w), height(h) {
        pixels.resize(w * h, Color(0.0, 0.0, 0.0));
    }

    Canvas::Canvas(size_t w, size_t h, Color c) : width(w), height(h), initialColor(c) {
        pixels.resize(w * h, initialColor);
    }

    void writePixel(Canvas &c, size_t x, size_t y, Color col) {
        if (x < c.width && y < c.height)
            c.pixels[(y * c.width) + x] = col;
    }

    Color pixelAt(const Canvas &c, size_t x, size_t y) {
        if (x < c.width && y < c.height)
            return c.pixels[(y * c.width) + x];
        return {1.0, 1.0, 1.0};
    }

    int scaleColor(double color_float) {
        double scaled = color_float * 255.0;
        double ceiled = std::ceil(scaled);
        return std::clamp(static_cast<int>(ceiled), 0, 255);
    }

    std::string canvasToPPM(const Canvas &c) {
        std::string ppm = std::format("P3\n{} {}\n255\n", c.width, c.height);

        for (size_t y = 0; y < c.height; ++y) {
            std::string currentLine;

            for (size_t x = 0; x < c.width; ++x) {
                Color col = pixelAt(c, x, y);

                std::array components = {
                    scaleColor(col.r),
                    scaleColor(col.g),
                    scaleColor(col.b)
                };

                for (size_t i = 0; i < 3; ++i) {
                    std::string s = std::to_string(components[i]);

                    if (currentLine.length() + s.length() + 1 > 70) {
                        ppm += currentLine + "\n";
                        currentLine = "";
                    }

                    if (!currentLine.empty()) {
                        currentLine += " ";
                    }

                    currentLine += s;
                }
            }
            if (!currentLine.empty()) {
                ppm += currentLine + "\n";
            }
        }

        return ppm;
    }

    void savePPM(const std::string& filename, const std::string& ppmData) {
        std::ofstream outFile(filename + ".ppm");

        if (!outFile.is_open()) {
            std::cerr << std::format("Error: Could not open file {}.ppm\n", filename);
            return;
        }

        outFile << ppmData;
        outFile.close();
        std::cout << std::format("Successfully saved to {}.ppm\n", filename);
    }

} // namespace rt
