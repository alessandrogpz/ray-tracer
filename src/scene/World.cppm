export module rt.world;

import std;
import rt.sphere;
import rt.lights;
import rt.intersection;
import rt.ray;
import rt.tuple;

export namespace rt
{
    struct World
    {
        std::vector<Sphere> spheres{};
        PointLight light{};

        World() = default;
    };

    struct Comp
    {
        Point point{};
        Vector eye_v{};
        Vector normal_v{};
        Intersection intersection{};

        Comp() = default;
    };

    [[nodiscard]] World default_world();
    [[nodiscard]] std::vector<Intersection> intersect_world(const World& w, const Ray& r);

    /**
     * @brief Precomputes relevant geometric vectors and state for a given intersection.
     *
     * Calculates the exact 3D point of intersection, the eye vector pointing back
     * towards the ray's origin, and the surface normal vector at that point.
     *
     * @param i The Intersection containing the t-value and shape identifiers.
     * @param r The Ray that caused the intersection.
     * @param w The World containing the scene objects.
     * @return Comp A struct containing the precomputed values.
     */
    [[nodiscard]] Comp prepare_computation(const Intersection& i, const Ray& r, const World& w);
}