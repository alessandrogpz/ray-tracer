// visualizers/3.ConsoleIntersectionCheck.cpp
#include <iostream>
#include <vector>

import rt.tuple;
import rt.ray;
import rt.sphere;
import rt.intersection;
import rt.transformations;
import rt.colors;

using namespace rt;

int main() {
    std::cout << "--- Ray Tracing Playground ---\n\n";

    // 1. Create a Ray
    // Starts at (0, 0, -5) and shoots straight forward along the Z-axis (0, 0, 1)
    Point ray_origin(0.0f, 0.0f, -5.0f);
    Vector ray_direction(0.0f, 0.0f, 1.0f);
    Ray r(ray_origin, ray_direction);

    std::cout << "Ray Origin: (" << r.origin.x << ", " << r.origin.y << ", " << r.origin.z << ")\n";
    std::cout << "Ray Direction: (" << r.direction.x << ", " << r.direction.y << ", " << r.direction.z << ")\n\n";

    // 2. Create two Spheres
    Sphere s1; // Default sphere at origin (0, 0, 0) with radius 1.0
    Sphere s2; // Second sphere

    // Let's translate the second sphere slightly to the side and scale it down
    s2.set_transform(translation(0.5f, 0.0f, 0.0f) * scale(0.5f, 0.5f, 0.5f));
    s2.material.color = Color(0.1f, 0.6f, 1.0f); // Blue material

    // 3. Perform intersections (using the high-performance in-place vector)
    std::vector<Intersection> xs;

    // Check intersections for s1 (we assign it index 0)
    intersect(s1, r, xs, 0);

    // Check intersections for s2 (we assign it index 1)
    intersect(s2, r, xs, 1);

    std::cout << "Total intersections found: " << xs.size() << "\n";
    for (const auto& i : xs) {
        std::cout << "  - Hit at distance t = " << i.t
                  << " on shape index = " << i.shape_index
                  << " (Type: " << (i.shape_type == ShapeType::Sphere ? "Sphere" : "Unknown") << ")\n";
    }
    std::cout << "\n";

    // 4. Find the closest hit (the one the camera actually sees)
    if (auto h = hit(xs)) {
        std::cout << "Closest visible hit is at t = " << h->t << "\n";

        // Resolve the hit object by looking up its index
        const Sphere& hit_sphere = (h->shape_index == 0) ? s1 : s2;

        // Check its material color
        std::cout << "Hit Sphere Material Color: RGB("
                  << hit_sphere.material.color.r << ", "
                  << hit_sphere.material.color.g << ", "
                  << hit_sphere.material.color.b << ")\n";
    } else {
        std::cout << "The ray missed everything!\n";
    }

    return 0;
}