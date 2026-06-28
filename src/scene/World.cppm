export module rt.world;

import std;
import rt.sphere;
import rt.lights;
import rt.intersection;
import rt.materials;
import rt.matrix;
import rt.ray;
import rt.tuple;
import rt.colors;
import rt.plane;

export namespace rt
{
    struct World
    {
        // Spheres SoA
        std::vector<Point> sphere_origins{};
        std::vector<float> sphere_radii{};
        std::vector<Material> sphere_materials{};
        std::vector<Matrix<4>> sphere_transforms{};
        std::vector<Matrix<4>> sphere_transforms_inverse{};
        std::vector<Matrix<4>> sphere_transforms_inverse_transpose{};

        // Planes SoA
        std::vector<Material> plane_materials{};
        std::vector<Matrix<4>> plane_transforms{};
        std::vector<Matrix<4>> plane_transforms_inverse{};
        std::vector<Matrix<4>> plane_transforms_inverse_transpose{};

        PointLight light{};

        World() = default;

        /**
         * @brief Adds a Sphere to the World, splitting it into parallel SoA vectors.
         */
        void add_sphere(const Sphere& s);

        /**
         * @brief Adds a Plane to the World, splitting it into parallel SoA vectors.
         */
        void add_plane(const Plane& p);
    };

    struct Comp
    {
        Point point{};
        Point over_point{};
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
     * towards the ray's origin, the surface normal vector at that point, checks
     * if the intersection occurred on the inside of the shape (inverting the normal if so),
     * and calculates the offset `over_point` to prevent shadow acne.
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
     * Resolves the material of the intersected shape from the world, checks
     * whether the intersection point is in shadow, and calculates the cumulative
     * Phong lighting using the world's light source.
     *
     * @param w The World containing the scene light and object arrays.
     * @param c The Comp struct holding precomputed geometric and intersection vectors.
     * @return Color The final calculated Color for the hit.
     */
    [[nodiscard]] Color shade_hit(const World& w, const Comp& c);

    /**
     * @brief Computes the Color in the World as seen by a given Ray.
     *
     * Finds the closest visible intersection of the Ray with objects in the World,
     * precomputes the intersection details, and calculates the shaded Color.
     * Returns black if no intersection occurs.
     *
     * @param w The World containing scene elements.
     * @param r The Ray representing the light path.
     * @return Color The final calculated Color seen by the Ray.
     */
    [[nodiscard]] Color color_at(const World& w, const Ray& r);
    /**
     * @brief Determines whether a given 3D point on a surface is in shadow.
     *
     * Traces a shadow ray from the point toward the light source. If any opaque
     * object is found obstructing the light (closer than the light source itself),
     * the point is considered shadowed.
     *
     * @param w The World containing scene elements and the light source.
     * @param p The 3D Point on the surface to be tested (should be comps.over_point to avoid acne).
     * @return true If the point is in shadow.
     * @return false If the point is directly illuminated.
     */
    [[nodiscard]] bool is_shadowed(const World& w, const Point& p);
}