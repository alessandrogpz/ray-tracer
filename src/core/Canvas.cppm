module;

#include <vector>
#include <string>

export module rt.canvas;

import rt.colors;

export namespace rt {

    struct Canvas {
        size_t width;
        size_t height;
        Color initialColor;
        std::vector<Color> pixels;

        Canvas(size_t w, size_t h);
        Canvas(size_t w, size_t h, Color c);
    };

    void writePixel(Canvas &c, size_t x, size_t y, Color col);
    [[nodiscard]] Color pixelAt(const Canvas &c, size_t x, size_t y);
    [[nodiscard]] int scaleColor(double color_float);
    [[nodiscard]] std::string canvasToPPM(const Canvas &c);
    void savePPM(const std::string& filename, const std::string& ppmData);

} // namespace rt
