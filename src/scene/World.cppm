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

    /**
     * @brief Default number of secondary rays (reflection/refraction bounces) a primary ray may spawn.
     *
     * Bounds the color_at → shade_hit → reflected_color/refracted_color recursion, so two
     * facing mirrors cannot recurse forever.
     */
    constexpr int MAX_RECURSION_DEPTH = 5;

    struct Comp
    {
        Point point{};
        Point over_point{};                 ///< Slightly above the surface; origin for shadow and reflection rays
        Point under_point{};                ///< Slightly below the surface; origin for refraction rays
        Vector eye_v{};
        Vector normal_v{};
        Vector reflect_v{};                 ///< The incoming ray reflected around the surface normal
        Intersection intersection{};
        float n1{1.0f};                     ///< Refractive index of the material being exited
        float n2{1.0f};                     ///< Refractive index of the material being entered
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
     * and calculates the offset `over_point` (to prevent shadow acne) and `under_point`
     * (the origin of refracted rays). It also reflects the ray around the normal (`reflect_v`).
     *
     * The full intersection list `xs` is walked to find which objects contain the hit,
     * yielding the refractive indices `n1` (exited) and `n2` (entered).
     * Math reference: documentation/explanation/ReflectionAndRefraction.md
     *
     * @param i The hit Intersection containing the t-value and shape identifiers.
     * @param r The Ray that caused the intersection.
     * @param w The World containing the scene objects.
     * @param xs Every intersection along the ray, sorted by t (must contain `i`).
     * @return Comp A struct containing the precomputed values.
     */
    [[nodiscard]] Comp prepare_computation(const Intersection& i, const Ray& r, const World& w, const std::vector<Intersection>& xs);

    /**
     * @brief Convenience overload that treats `i` as the only intersection along the ray.
     *
     * Suitable when refraction is irrelevant; n1 and n2 are derived from `i` alone.
     */
    [[nodiscard]] Comp prepare_computation(const Intersection& i, const Ray& r, const World& w);

    /**
     * @brief Computes the shaded Color at a given ray-object intersection.
     *
     * Resolves the material of the intersected shape from the world, checks
     * whether the intersection point is in shadow, and calculates the cumulative
     * Phong lighting using the world's light source. The reflected and refracted
     * colors are added on top; when the material is both reflective and transparent,
     * they are weighted with the Schlick approximation of the Fresnel effect.
     * Math reference: documentation/explanation/ReflectionAndRefraction.md
     *
     * @param w The World containing the scene light and object arrays.
     * @param c The Comp struct holding precomputed geometric and intersection vectors.
     * @param remaining How many more secondary rays may be spawned from this hit.
     * @return Color The final calculated Color for the hit.
     */
    [[nodiscard]] Color shade_hit(const World& w, const Comp& c, int remaining = MAX_RECURSION_DEPTH);

    /**
     * @brief Computes the Color in the World as seen by a given Ray.
     *
     * Finds the closest visible intersection of the Ray with objects in the World,
     * precomputes the intersection details, and calculates the shaded Color.
     * Returns black if no intersection occurs.
     *
     * @param w The World containing scene elements.
     * @param r The Ray representing the light path.
     * @param remaining How many more secondary rays may be spawned (recursion budget).
     * @return Color The final calculated Color seen by the Ray.
     */
    [[nodiscard]] Color color_at(const World& w, const Ray& r, int remaining = MAX_RECURSION_DEPTH);

    /**
     * @brief Computes the color contributed by a mirror reflection at a hit.
     *
     * Spawns a secondary ray from `over_point` along `reflect_v`, traces it through the
     * World, and scales the result by the material's `reflective` value.
     * Returns black for non-reflective materials or when the recursion budget is exhausted.
     * Math reference: documentation/explanation/ReflectionAndRefraction.md
     *
     * @param w The World containing scene elements.
     * @param c The precomputed hit state.
     * @param remaining How many more secondary rays may be spawned.
     * @return Color The reflected Color contribution.
     */
    [[nodiscard]] Color reflected_color(const World& w, const Comp& c, int remaining = MAX_RECURSION_DEPTH);

    /**
     * @brief Computes the color contributed by light refracting through a transparent hit.
     *
     * Uses Snell's law (with n1 and n2) to bend the ray, spawns it from `under_point`,
     * traces it through the World, and scales the result by the material's `transparency`.
     * Returns black for opaque materials, under total internal reflection, or when the
     * recursion budget is exhausted.
     * Math reference: documentation/explanation/ReflectionAndRefraction.md
     *
     * @param w The World containing scene elements.
     * @param c The precomputed hit state (including n1 and n2).
     * @param remaining How many more secondary rays may be spawned.
     * @return Color The refracted Color contribution.
     */
    [[nodiscard]] Color refracted_color(const World& w, const Comp& c, int remaining = MAX_RECURSION_DEPTH);

    /**
     * @brief Approximates the Fresnel reflectance at a hit with Christophe Schlick's formula.
     *
     * Returns the fraction of light that is reflected (0 to 1): 1 under total internal
     * reflection, small when looking straight at the surface, and larger at grazing angles.
     * Math reference: documentation/explanation/ReflectionAndRefraction.md
     *
     * @param c The precomputed hit state (eye vector, normal, n1 and n2).
     * @return float The reflectance.
     */
    [[nodiscard]] float schlick(const Comp& c);
    /**
     * @brief Determines whether a given 3D point on a surface is in shadow.
     *
     * Traces a shadow ray from the point toward the light source. If any object that
     * casts shadows is found obstructing the light (closer than the light source itself),
     * the point is considered shadowed. Objects whose material sets `casts_shadow = false`
     * (e.g. a water surface) are ignored, so light reaches what lies behind them.
     *
     * @param w The World containing scene elements and the light source.
     * @param p The 3D Point on the surface to be tested (should be comps.over_point to avoid acne).
     * @return true If the point is in shadow.
     * @return false If the point is directly illuminated.
     */
    [[nodiscard]] bool is_shadowed(const World& w, const Point& p);
}