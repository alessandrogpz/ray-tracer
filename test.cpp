#include "Canvas.hpp"
#include "Colors.hpp"
#include "Tuple.hpp"

/**
 * Manual compilation:
 *  g++ -std=c++20 main.cpp src/Canvas.cpp src/Colors.cpp src/Tuple.cpp src/Utils.cpp  
 */


int main(int argc, char **argv)
{
    color red(1, 0, 0);
    canvas c(100, 100, red);

    std::string ppmContent = canvasToPPM(c);

    savePPM("test_output", ppmContent);

    return 0;
}