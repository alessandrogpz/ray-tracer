#include <string>
#include <numbers>
#include <chrono>
#include <iostream>

import rt.utils;
import rt.tuple;
import rt.transformations;
import rt.canvas;
import rt.matrix;
import rt.colors;

using namespace rt;

void writePixelBlock(Canvas &c, int h, int w, int startX, int startY, Color col) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            std::size_t drawX = startX + j;
            std::size_t drawY = startY + i;
            if (drawX < c.width && drawY < c.height) {
                writePixel(c, drawX, drawY, col);
            }
        }
    }
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    Color yellow(1, 1, 0);
    constexpr std::size_t CANVAS_WIDTH = 2560;
    constexpr std::size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT);

    double radius = static_cast<double>(CANVAS_HEIGHT) * 0.4;
    Point marker = createPoint(0.0, radius, 0.0);
    Matrix<4> rot = rotation_z(std::numbers::pi / 6.0);

    for (int i = 0; i < 12; i++)
    {
        int x = static_cast<int>(marker.x + static_cast<double>(CANVAS_WIDTH) / 2.0);
        int y = static_cast<int>(marker.y + static_cast<double>(CANVAS_HEIGHT) / 2.0);

        writePixelBlock(c, 8, 8, x - 4, y - 4, yellow);

        marker = rot * marker;
    }

    std::string ppmContent = canvasToPPM(c);

    savePPM("OutputClockMarkers", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
