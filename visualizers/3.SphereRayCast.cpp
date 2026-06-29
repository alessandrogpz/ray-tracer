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
    Color black(0.0f, 0.0f, 0.0f);
    Color red(1.0f, 0.0f, 0.0f);

    constexpr std::size_t CANVAS_WIDTH = 2560;
    constexpr std::size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT, black);

    Point ray_origin = Point(0.0f, 0.0f, -10.0f);
    float wall_z = 10.0f;
    float wall_size = 7.0f;

    float aspect_ratio = static_cast<float>(CANVAS_WIDTH) / CANVAS_HEIGHT;
    float wall_height = wall_size;
    float wall_width = wall_size * aspect_ratio;

    float pixel_size = wall_height / CANVAS_HEIGHT;
    float half_width = wall_width / 2.0f;
    float half_height = wall_height / 2.0f;

    Sphere s = Sphere();

    // Iterate over every row (y) and column (x) of the Canvas
    for (std::size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate (top = +half_height, bottom = -half_height)
        float world_y = half_height - pixel_size * y;

        std::vector<Intersection> xs;
        xs.reserve(2);

        for (std::size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Compute the world x coordinate (left = -half_width, right = +half_width)
            float world_x = -half_width + pixel_size * x;

            // Define the Point on the wall that the Ray will target
            Point position = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position - ray_origin);
            Ray r(ray_origin, direction);

            // Cast the Ray and check for intersections
            xs.clear();
            intersect(s, r, xs);

            // If a valid hit occurs, Color the pixel red
            if (auto h = hit(xs))
            {
                writePixel(c, x, y, red);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputSphereRayCast", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
