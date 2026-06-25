//
// Created by aper on 25.06.26.
//

#include <string>
#include <optional>
#include <vector>
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
import rt.lights;
import rt.materials;
import rt.shading;
import rt.transformations;

using namespace rt;

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    // Background color (very dark gray to make the sphere pop)
    Color background_color(0.05, 0.05, 0.05);

    constexpr std::size_t CANVAS_WIDTH = 2560;
    constexpr std::size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT, background_color);

    Point ray_origin = Point(0.0, 0.0, -5.0);
    double wall_z = 10.0;
    double wall_size = 7.0;

    double aspect_ratio = static_cast<double>(CANVAS_WIDTH) / CANVAS_HEIGHT;
    double wall_height = wall_size;
    double wall_width = wall_size * aspect_ratio;

    double pixel_size = wall_height / CANVAS_HEIGHT;
    double half_width = wall_width / 2.0;
    double half_height = wall_height / 2.0;

    // Create a sphere and customize its Material
    Sphere s1 = Sphere();
    Sphere s2 = Sphere();
    Sphere s3 = Sphere();

    Matrix<4> transform_s1 = translation(0.0, 0.0, 0.0);
    s1.set_transform(transform_s1);

    Matrix<4> transform_s2 = translation(0.5, 0.5, -0.5) * scale(0.5, 0.5, 0.5);
    s2.set_transform(transform_s2);

    Matrix<4> transform_s3 = translation(-0.5, -0.5, -0.5) * scale(0.5, 0.5, 0.5);
    s3.set_transform(transform_s3);

    s1.material.color = Color(1.0, 0.2, 1.0); // Vibrant magenta
    s1.material.diffuse = 0.9;
    s1.material.specular = 0.9;
    s1.material.shininess = 200.0;

    s2.material.color = Color(0.1, 0.6, 1.0); // Cool blue
    s2.material.diffuse = 0.9;
    s2.material.specular = 0.9;
    s2.material.shininess = 200.0;

    s3.material.color = Color(1.0, 0.8, 0.1); // Warm yellow
    s3.material.diffuse = 0.9;
    s3.material.specular = 0.9;
    s3.material.shininess = 200.0;

    // A white point light source, positioned behind, above, and to the left of the eye
    PointLight light(Point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

    #pragma omp parallel for
    // Iterate over every row (y) and column (x) of the Canvas
    for (std::size_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        // Compute the world y coordinate (top = +half_height, bottom = -half_height)
        double world_y = half_height - pixel_size * y;

        std::vector<Intersection> xs;
        xs.reserve(6);

        for (std::size_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            // Compute the world x coordinate (left = -half_width, right = +half_width)
            double world_x = -half_width + pixel_size * x;

            // Define the Point on the wall that the Ray will target
            Point position_on_wall = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position_on_wall - ray_origin);
            Ray r(ray_origin, direction);

            // Cast the Ray and check for intersections
            xs.clear();
            intersect(s1, r, xs);
            intersect(s2, r, xs);
            intersect(s3, r, xs);

            // If a valid hit occurs, calculate lighting and Color the pixel
            if (auto h = hit(xs))
            {
                Point p = position(r, h->t);
                Vector normal = normalAt(*(h->obj), p);
                Vector eye = -r.direction;

                Color pixel_color = lighting(h->obj->material, light, p, eye, normal);
                writePixel(c, x, y, pixel_color);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputMultipleSpherePhongReflections", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
