#include <string>
#include <numbers>

import rt.tuple;
import rt.utils;
import rt.canvas;
import rt.colors;
import rt.intersection;
import rt.matrix;
import rt.shapes;
import rt.ray;
import rt.transformations;

using namespace rt;

int main()
{
    Color black(0.0, 0.0, 0.0);
    Color red(1.0, 0.0, 0.0);

    constexpr size_t CANVAS_SIZE = 250;
    Canvas c(CANVAS_SIZE, CANVAS_SIZE, black);

    Point ray_origin = Point(0.0, 0.0, -10.0);
    double wall_z = 10.0;
    double wall_size = 7.0;

    double pixel_size = wall_size / CANVAS_SIZE;
    double half = wall_size / 2.0;

    Sphere s = Sphere();

    // Iterate over every row (y) and column (x) of the Canvas
    for (size_t y = 0; y < CANVAS_SIZE; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half)
        double world_y = half - pixel_size * y;

        for (size_t x = 0; x < CANVAS_SIZE; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half)
            double world_x = -half + pixel_size * x;

            // Define the Point on the wall that the Ray will target
            Point position = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position - ray_origin);
            Ray r(ray_origin, direction);

            // Cast the Ray and check for intersections
            auto xs = intersect(s, r);

            // If a valid hit occurs, Color the pixel red
            if (auto h = hit(xs))
            {
                writePixel(c, x, y, red);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputSphereShadow", ppmContent);

    return 0;
}
