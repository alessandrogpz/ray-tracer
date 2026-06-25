#include <string>
#include <numbers>
#include <chrono>
#include <iostream>

import rt.tuple;
import rt.utils;
import rt.canvas;
import rt.colors;
import rt.intersection;
import rt.matrix;
import rt.sphere;
import rt.ray;
import rt.transformations;

using namespace rt;

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    Color black(0.0, 0.0, 0.0);
    Color red(1.0, 0.0, 0.0);

    constexpr size_t CANVAS_WIDTH = 2560;
    constexpr size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT, black);

    Point ray_origin = Point(0.0, 0.0, -10.0);
    double wall_z = 10.0;
    double wall_size = 7.0;

    double aspect_ratio = static_cast<double>(CANVAS_WIDTH) / CANVAS_HEIGHT;
    double wall_height = wall_size;
    double wall_width = wall_size * aspect_ratio;

    double pixel_size = wall_height / CANVAS_HEIGHT;
    double half_width = wall_width / 2.0;
    double half_height = wall_height / 2.0;

    Sphere s = Sphere();

    // Iterate over every row (y) and column (x) of the Canvas
    for (size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate (top = +half_height, bottom = -half_height)
        double world_y = half_height - pixel_size * y;

        for (size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Compute the world x coordinate (left = -half_width, right = +half_width)
            double world_x = -half_width + pixel_size * x;

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

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
