//
// Created by aper on 25.06.26.
//

#include <string>
#include <optional>
#include <vector>

import rt.tuple;
import rt.utils;
import rt.canvas;
import rt.colors;
import rt.intersection;
import rt.matrix;
import rt.shapes;
import rt.ray;
import rt.lights;
import rt.materials;
import rt.shading;

using namespace rt;

int main()
{
    // Background color (very dark gray to make the sphere pop)
    Color background_color(0.05, 0.05, 0.05);

    constexpr size_t CANVAS_SIZE = 500;
    Canvas c(CANVAS_SIZE, CANVAS_SIZE, background_color);

    Point ray_origin = Point(0.0, 0.0, -5.0);
    double wall_z = 10.0;
    double wall_size = 7.0;

    double pixel_size = wall_size / CANVAS_SIZE;
    double half = wall_size / 2.0;

    // Create a single sphere and customize its Material
    Sphere s = Sphere();
    s.material.color = Color(1.0, 0.2, 1.0); // Vibrant magenta
    s.material.diffuse = 0.9;
    s.material.specular = 0.9;
    s.material.shininess = 200.0;

    // A white point light source, positioned behind, above, and to the left of the eye
    PointLight light(Point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

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
            Point position_on_wall = Point(world_x, world_y, wall_z);

            // Calculate the direction of the Ray (target - origin)
            Vector direction = normalizeVector(position_on_wall - ray_origin);
            Ray r(ray_origin, direction);

            // Cast the Ray and check for intersections
            auto xs = intersect(s, r);

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
    savePPM("OutputSpherePhongReflection", ppmContent);

    return 0;
}
