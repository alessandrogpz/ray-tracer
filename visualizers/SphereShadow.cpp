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
    color black(0.0f, 0.0f, 0.0f);
    color red(1.0f, 0.0f, 0.0f);
    color blue(0.0f, 0.0f, 1.0f);
    color yellow(1.0f, 1.0f, 0.0f);

    constexpr size_t CANVAS_SIZE = 250;
    canvas c(CANVAS_SIZE, CANVAS_SIZE, black);

    point ray_origin = point(0.0f, 0.0f, -10.0f);
    float wall_z = 10.0f;
    float wall_size = 7.0f;

    float pixel_size = wall_size / CANVAS_SIZE;
    float half = wall_size / 2.0f;

    sphere s1 = sphere();
    sphere s2 = sphere();
    sphere s3 = sphere();

    matrix<4> transform_s1 = translation(0.0f, 0.0f, 0.0f);
    s1.set_transform(transform_s1);

    matrix<4> transform_s2 = translation(0.5f, 0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s2.set_transform(transform_s2);

    matrix<4> transform_s3 = translation(-0.5f, -0.5f, -0.5f) * scale(0.5f, 0.5f, 0.5f);
    s3.set_transform(transform_s3);

    // Iterate over every row (y) and column (x) of the canvas
    // #pragma omp parallel for // Add multithreading support
    for (size_t y = 0; y < CANVAS_SIZE; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half)
        float world_y = half - pixel_size * y;

        for (size_t x = 0; x < CANVAS_SIZE; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half)
            float world_x = -half + pixel_size * x;

            // Define the point on the wall that the ray will target
            point position = point(world_x, world_y, wall_z);

            // Calculate the direction of the ray (target - origin)
            vector direction = normalizeVector(position - ray_origin);
            ray r(ray_origin, direction);

            // Cast the ray and check for intersections
            auto xs = intersect(s1, r);

            auto xs2 = intersect(s2, r);
            xs.insert(xs.end(), xs2.begin(), xs2.end());

            auto xs3 = intersect(s3, r);
            xs.insert(xs.end(), xs3.begin(), xs3.end());

            // If a valid hit occurs, color the pixel with the appropriate object color
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
    savePPM("OutputSphereShadow", ppmContent);

    return 0;
}