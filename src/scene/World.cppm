export module rt.world;

import std;
import rt.sphere;
import rt.lights;
import rt.intersection;
import rt.ray;

export namespace rt
{
    struct World
    {
        std::vector<Sphere> spheres{};
        PointLight light;

        World() = default;
    };

    [[nodiscard]] World default_world();
    [[nodiscard]] std::vector<Intersection> intersect_world(const World& w, const Ray& r);
}