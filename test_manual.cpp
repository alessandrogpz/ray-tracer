#include <string>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

/**
 * Manual compilation:
 * g++ -std=c++20 -fmodules-ts src/Utils.cppm src/Tuple.cppm src/Colors.cppm src/Canvas.cppm src/Matrix.cppm src/Transformations.cppm test.cpp -o test_bin 
 */


int main(int argc, char **argv)
{
    color red(1, 0, 0);
    canvas c(100, 100, red);

    std::string ppmContent = canvasToPPM(c);

    savePPM("test_output", ppmContent);

    return 0;
}