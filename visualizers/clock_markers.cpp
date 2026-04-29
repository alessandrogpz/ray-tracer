#include <string>
#include <numbers>

import rt.utils;
import rt.tuple;
import rt.transformations;
import rt.canvas;
import rt.matrix;
import rt.colors;

using namespace rt;

int main()
{
    color yellow(1, 1, 0);
    canvas c(100, 100);

    tuple marker = createPoint(0.0f, 40.0f, 0.0f);
    matrix<4> rot = rotation_z(std::numbers::pi / 6.0f);

    for (int i = 0; i < 12; i++)
    {
        int x = static_cast<int>(marker.x + 50);
        int y = static_cast<int>(marker.y + 50);

        writePixel(c, x, y, yellow);

        marker = rot * marker;
    }

    std::string ppmContent = canvasToPPM(c);

    savePPM("clock_markers", ppmContent);

    return 0;
}
