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
    Color blue(0.0f, 0.0f, 1.0f);
    Color yellow(1.0f, 1.0f, 0.0f);

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

    Sphere s1 = Sphere();
    Sphere s2 = Sphere();
    Sphere s3 = Sphere();

    Matrix<4> transform_s1 = translation(0.0f, 0.0f, 0.0f);
    s1.set_transform(transform_s1);

    Matrix<4> transform_s2 = translation(0.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s2.set_transform(transform_s2);

    Matrix<4> transform_s3 = translation(-0.5f, -0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s3.set_transform(transform_s3);

    // Iterate over every row (y) and column (x) of the Canvas
    for (std::size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate (top = +half_height, bottom = -half_height)
        float world_y = half_height - pixel_size * y;

        std::vector<Intersection> xs;
        xs.reserve(6);

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
            intersect(s1, r, xs, 0);
            intersect(s2, r, xs, 1);
            intersect(s3, r, xs, 2);

            // If a valid hit occurs, Color the pixel with the appropriate object Color
            if (auto h = hit(xs))
            {
                if (h->shape_index == 0)
                    writePixel(c, x, y, red);
                else if (h->shape_index == 2)
                    writePixel(c, x, y, yellow);
                else if (h->shape_index == 1)
                    writePixel(c, x, y, blue);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputMultipleSphereShadows", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}