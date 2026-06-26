export module rt.world;

import std;
import rt.sphere;
import rt.lights;

export namespace rt
{
    struct World
    {
        std::vector<Sphere> spheres{};
        PointLight light;

        World() = default;
    };

    [[nodiscard]] World default_world();
}