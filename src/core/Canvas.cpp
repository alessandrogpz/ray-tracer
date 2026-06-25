module rt.canvas;

import std;

import rt.colors;

namespace rt {

    Canvas::Canvas(std::size_t w, std::size_t h) : width(w), height(h) {
        pixels.resize(w * h, PixelRGBA8{0, 0, 0, 255});
    }

    Canvas::Canvas(std::size_t w, std::size_t h, Color c) : width(w), height(h), initialColor(c) {
        PixelRGBA8 default_pixel{
            static_cast<std::uint8_t>(scaleColor(c.r)),
            static_cast<std::uint8_t>(scaleColor(c.g)),
            static_cast<std::uint8_t>(scaleColor(c.b)),
            static_cast<std::uint8_t>(scaleColor(c.a))
        };
        pixels.resize(w * h, default_pixel);
    }

    // Converts a Color to 4-byte PixelRGBA8 on write
    void writePixel(Canvas &c, std::size_t x, std::size_t y, Color col) {
        if (x < c.width && y < c.height) {
            c.pixels[(y * c.width) + x] = PixelRGBA8{
                static_cast<std::uint8_t>(scaleColor(col.r)),
                static_cast<std::uint8_t>(scaleColor(col.g)),
                static_cast<std::uint8_t>(scaleColor(col.b)),
                static_cast<std::uint8_t>(scaleColor(col.a))
            };
        }
    }

    // Converts PixelRGBA8 back to HDR Color on read
    Color pixelAt(const Canvas &c, std::size_t x, std::size_t y) {
        if (x < c.width && y < c.height) {
            const PixelRGBA8 &p = c.pixels[(y * c.width) + x];
            return Color(
                static_cast<double>(p.r) / 255.0,
                static_cast<double>(p.g) / 255.0,
                static_cast<double>(p.b) / 255.0,
                static_cast<double>(p.a) / 255.0
            );
        }
        return Color(1.0, 1.0, 1.0, 1.0); // Opaque white boundary fallback
    }

    int scaleColor(double color_float) {
        double scaled = color_float * 255.0;
        double ceiled = std::ceil(scaled);
        return std::clamp(static_cast<int>(ceiled), 0, 255);
    }

    // Fast PPM conversion (Direct read from byte array, no float math)
    std::string canvasToPPM(const Canvas &c) {
        std::string ppm = std::format("P3\n{} {}\n255\n", c.width, c.height);
        for (std::size_t y = 0; y < c.height; ++y) {
            std::string currentLine;
            for (std::size_t x = 0; x < c.width; ++x) {
                const PixelRGBA8 &p = c.pixels[(y * c.width) + x];
                std::array components = {
                    static_cast<int>(p.r),
                    static_cast<int>(p.g),
                    static_cast<int>(p.b)
                };
                for (std::size_t i = 0; i < 3; ++i) {
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
