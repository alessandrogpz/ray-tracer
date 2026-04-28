import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

/**
 * Manual compilation:
 *  g++ -std=c++20 test.cpp src/Canvas.cpp src/Colors.cpp src/Tuple.cpp src/Utils.cpp  
 */


int main(int argc, char **argv)
{
    color red(1, 0, 0);
    canvas c(100, 100, red);

    std::string ppmContent = canvasToPPM(c);

    savePPM("test_output", ppmContent);

    return 0;
}