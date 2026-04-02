#include "../includes/Canvas.hpp"
#include <cassert>

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