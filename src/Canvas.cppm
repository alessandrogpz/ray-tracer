module;

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <format>
#include <print>

export module rt.canvas;

import rt.colors;

export namespace rt {

    struct canvas {
        size_t width;
        size_t height;
        color initialColor;
        std::vector<color> pixels;

        // 2D representation in 1D array
        // index = (y * width) + x

        canvas(size_t w, size_t h) : width(w), height(h) {
            //Initialize the vector with w * h black colors
            pixels.resize(w * h, color(0, 0, 0));
        }

        canvas(size_t w, size_t h, color c) : width(w), height(h), initialColor(c) {
            //Initialize the vector with w * h with initialColor
            pixels.resize(w * h, initialColor);
        }
    };

    void writePixel(canvas &c, size_t x, size_t y, color col)
    {
        // Basic canvas boundary check
        if(x < c.width && y < c.height)
            c.pixels[(y * c.width) + x] = col;
    }

    [[nodiscard]] color pixelAt(const canvas &c, size_t x, size_t y)
    {
        // Basic canvas boundary check
        if(x < c.width && y < c.height)
            return c.pixels[(y * c.width) + x];

        return {1,1,1};
    }

    int scaleColor(float color_float) {
        // 1. Scale by 255
        float scaled = color_float * 255.0f;

        // 2. Apply ceiling
        float ceiled = std::ceil(scaled);

        // 3. Cast to int and clamp between 0 and 255
        return std::clamp(static_cast<int>(ceiled), 0, 255);
    }

    [[nodiscard]] std::string canvasToPPM(const canvas &c) {
        // 1. Header
        std::string ppm = std::format("P3\n{} {}\n255\n", c.width, c.height);

        // 2. Pixel Data
        for (size_t y = 0; y < c.height; ++y) {
            std::string currentLine;

            for (size_t x = 0; x < c.width; ++x) {
                color col = pixelAt(c, x, y);

                std::array components = {
                    scaleColor(col.r),
                    scaleColor(col.g),
                    scaleColor(col.b)
                };

                for (size_t i = 0; i < 3; ++i) {
                    std::string s = std::to_string(components[i]);

                    // PPM line formater
                    if (currentLine.length() + s.length() + 1 > 70) {
                        ppm += currentLine + "\n";
                        currentLine = "";
                    }

                    // Add a space if the line isn't empty
                    if (!currentLine.empty()) {
                        currentLine += " ";
                    }

                    currentLine += s;
                }
            }
            // 3. End of a canvas with \n row
            if (!currentLine.empty()) {
                ppm += currentLine + "\n";
            }
        }

        return ppm;
    }

    void savePPM(const std::string& filename, const std::string& ppmData)
    {
        std::ofstream outFile(filename + ".ppm");

        if(!outFile.is_open()) {
            std::println(std::cerr, "Error: Could not open file {}.ppm", filename);
            return;
        }

        outFile << ppmData;

        outFile.close();
        std::println("Successfully saved to {}.ppm", filename);
    }

} // namespace rt
