#include "../includes/Canvas.hpp"
#include <cassert>
#include <format>
#include <algorithm>
#include <cmath>
#include <array>

void writePixel(canvas &c, int x, int y, color col)
{
    // Basic canvas boundary check
    assert(x >= 0 && x < c.width && y >= 0 && y < c.height);
        c.pixels[(y * c.width) + x] = col;
}

color pixelAt(const canvas &c, int x, int y)
{
    // Basic canvas boundary check
    assert(x >= 0 && x < c.width && y >= 0 && y < c.height);
    
    return c.pixels[(y * c.width) + x];
}

int scaleColor(float color_float) {
    // 1. Scale by 255
    float scaled = color_float * 255.0f;
    
    // 2. Apply ceiling
    float ceiled = std::ceil(scaled);
    
    // 3. Cast to int and clamp between 0 and 255
    return std::clamp(static_cast<int>(ceiled), 0, 255);
}

std::string canvasToPPM(const canvas &c) {
    // 1. Header
    std::string ppm = std::format("P3\n{} {}\n255\n", c.width, c.height);

    // 2. Pixel Data
    for (int y = 0; y < c.height; ++y) {
        std::string currentLine = "";
        
        for (int x = 0; x < c.width; ++x) {
            color col = pixelAt(c, x, y);
            
            std::array<int, 3> components = { 
                scaleColor(col.r), 
                scaleColor(col.g), 
                scaleColor(col.b) 
            };

            for (int i = 0; i < 3; ++i) {
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