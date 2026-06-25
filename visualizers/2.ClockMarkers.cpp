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

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    Color yellow(1, 1, 0);
    Canvas c(100, 100);

    Point marker = createPoint(0.0, 40.0, 0.0);
    Matrix<4> rot = rotation_z(std::numbers::pi / 6.0);

    for (int i = 0; i < 12; i++)
    {
        int x = static_cast<int>(marker.x + 50);
        int y = static_cast<int>(marker.y + 50);

        writePixel(c, x, y, yellow);

        marker = rot * marker;
    }

    std::string ppmContent = canvasToPPM(c);

    savePPM("OutputClockMarkers", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
