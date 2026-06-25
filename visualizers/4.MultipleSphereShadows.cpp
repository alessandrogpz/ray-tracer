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
    Color blue(0.0, 0.0, 1.0);
    Color yellow(1.0, 1.0, 0.0);

    constexpr std::size_t CANVAS_WIDTH = 2560;
    constexpr std::size_t CANVAS_HEIGHT = 1440;
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

    Sphere s1 = Sphere();
    Sphere s2 = Sphere();
    Sphere s3 = Sphere();

    Matrix<4> transform_s1 = translation(0.0, 0.0, 0.0);
    s1.set_transform(transform_s1);

    Matrix<4> transform_s2 = translation(0.5, 0.5, -0.5) * scale(0.5, 0.5, 0.5);
    s2.set_transform(transform_s2);

    Matrix<4> transform_s3 = translation(-0.5, -0.5, -0.5) * scale(0.5, 0.5, 0.5);
    s3.set_transform(transform_s3);

    // Iterate over every row (y) and column (x) of the Canvas
    // #pragma omp parallel for // Add multithreading support
    for (std::size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate (top = +half_height, bottom = -half_height)
        double world_y = half_height - pixel_size * y;

        for (std::size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Compute the world x coordinate (left = -half_width, right = +half_width)
            double world_x = -half_width + pixel_size * x;

            // Define the Point on the wall that the Ray will target
            Point position = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position - ray_origin);
            Ray r(ray_origin, direction);

            // Cast the Ray and check for intersections
            auto xs = intersect(s1, r);

            auto xs2 = intersect(s2, r);
            xs.insert(xs.end(), xs2.begin(), xs2.end());

            auto xs3 = intersect(s3, r);
            xs.insert(xs.end(), xs3.begin(), xs3.end());

            // If a valid hit occurs, Color the pixel with the appropriate object Color
            if (auto h = hit(xs))
            {
                if (h->obj == &s1)
                    writePixel(c, x, y, red);
                else if (h->obj == &s3)
                    writePixel(c, x, y, yellow);
                else if (h->obj == &s2)
                    writePixel(c, x, y, blue);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputMultipleSphereShadows", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}