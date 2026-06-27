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
    Color background_color(0.05f, 0.05f, 0.05f);

    constexpr std::size_t CANVAS_WIDTH = 2560;
    constexpr std::size_t CANVAS_HEIGHT = 1440;
    Canvas c(CANVAS_WIDTH, CANVAS_HEIGHT, background_color);

    Point ray_origin = Point(0.0f, 0.0f, -5.0f);
    float wall_z = 10.0f;
    float wall_size = 7.0f;

    float aspect_ratio = static_cast<float>(CANVAS_WIDTH) / CANVAS_HEIGHT;
    float wall_height = wall_size;
    float wall_width = wall_size * aspect_ratio;

    float pixel_size = wall_height / CANVAS_HEIGHT;
    float half_width = wall_width / 2.0f;
    float half_height = wall_height / 2.0f;

    // Create a sphere and customize its Material
    Sphere s1 = Sphere();
    Sphere s2 = Sphere();
    Sphere s3 = Sphere();

    Matrix<4> transform_s1 = translation(0.0f, 0.0f, 0.0f);
    s1.set_transform(transform_s1);

    Matrix<4> transform_s2 = translation(0.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s2.set_transform(transform_s2);

    Matrix<4> transform_s3 = translation(-0.5f, -0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s3.set_transform(transform_s3);

    s1.material.color = Color(1.0f, 0.2f, 1.0f); // Vibrant magenta
    s1.material.diffuse = 0.9f;
    s1.material.specular = 0.9f;
    s1.material.shininess = 200.0f;

    s2.material.color = Color(0.1f, 0.6f, 1.0f); // Cool blue
    s2.material.diffuse = 0.9f;
    s2.material.specular = 0.9f;
    s2.material.shininess = 200.0f;

    s3.material.color = Color(1.0f, 0.8f, 0.1f); // Warm yellow
    s3.material.diffuse = 0.9f;
    s3.material.specular = 0.9f;
    s3.material.shininess = 200.0f;

    // A white point light source, positioned behind, above, and to the left of the eye
    PointLight light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    #pragma omp parallel for
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
            Point position_on_wall = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position_on_wall - ray_origin);
            Ray r(ray_origin, direction);

            xs.clear();
            intersect(s1, r, xs, 0);
            intersect(s2, r, xs, 1);
            intersect(s3, r, xs, 2);

            // If a valid hit occurs, calculate lighting and Color the pixel
            if (auto h = hit(xs))
            {
                const Sphere& hit_sphere = (h->shape_index == 0) ? s1 : ((h->shape_index == 1) ? s2 : s3);
                Point p = position(r, h->t);
                Vector normal = normalAt(hit_sphere, p);
                Vector eye = -r.direction;

                Color pixel_color = lighting(hit_sphere.material, light, p, eye, normal);
                writePixel(c, x, y, pixel_color);
            }
        }
    }

    std::string ppmContent = canvasToPPM(c);
    savePPM("OutputMultipleSpherePhongReflections", ppmContent);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
    std::cout << "Execution time: " << elapsed.count() << " ms\n";

    return 0;
}
