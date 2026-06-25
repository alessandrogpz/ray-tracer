export module rt.canvas;

import std;

import rt.colors;

export namespace rt {

    struct PixelRGBA8 {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };

    struct Canvas {
        std::size_t width;
        std::size_t height;
        Color initialColor;
        std::vector<PixelRGBA8> pixels;

        Canvas(std::size_t w, std::size_t h);
        Canvas(std::size_t w, std::size_t h, Color c);
    };

    void writePixel(Canvas &c, std::size_t x, std::size_t y, Color col);
    [[nodiscard]] Color pixelAt(const Canvas &c, std::size_t x, std::size_t y);
    [[nodiscard]] int scaleColor(double color_float);
    [[nodiscard]] std::string canvasToPPM(const Canvas &c);
    void savePPM(const std::string& filename, const std::string& ppmData);

} // namespace rt
