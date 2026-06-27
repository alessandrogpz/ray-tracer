export module rt.world;

import std;
import rt.sphere;
import rt.lights;
import rt.intersection;
import rt.ray;
import rt.tuple;
import rt.colors;

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
        bool inside { false };

        Comp() = default;
    };

    [[nodiscard]] World default_world();
    [[nodiscard]] std::vector<Intersection> intersect_world(const World& w, const Ray& r);

    /**
     * @brief Precomputes relevant geometric vectors and state for a given intersection.
     *
     * Calculates the exact 3D point of intersection, the eye vector pointing back
     * towards the ray's origin, the surface normal vector at that point, and checks
     * if the intersection occurred on the inside of the shape (inverting the normal if so).
     *
     * @param i The Intersection containing the t-value and shape identifiers.
     * @param r The Ray that caused the intersection.
     * @param w The World containing the scene objects.
     * @return Comp A struct containing the precomputed values.
     */
    [[nodiscard]] Comp prepare_computation(const Intersection& i, const Ray& r, const World& w);

    /**
     * @brief Computes the shaded Color at a given ray-object intersection.
     *
     * Resolves the material of the intersected shape from the world, and
     * calculates the cumulative Phong lighting using the world's light source.
     *
     * @param w The World containing the scene light and object arrays.
     * @param c The Comp struct holding precomputed geometric and intersection vectors.
     * @return Color The final calculated Color for the hit.
     */
    [[nodiscard]] Color shade_hit(const World& w, const Comp& c);
}