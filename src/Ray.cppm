module;

#include <vector>
#include <cmath>
#include <ranges>
#include <algorithm>
#include <optional>

export module rt.ray;

import rt.tuple;
import rt.shapes;
import rt.intersection;
import rt.matrix;
import rt.transformations;

export namespace rt
{
    struct ray
    {
        point origin;
        vector direction;

        ray() : origin(createPoint(0.0f, 0.0f, 0.0f)),
                direction(createVector(0.0f, 0.0f, 0.0f)) {}

        ray(point _origin, vector _direction)
            : origin(_origin), direction(_direction) {}
    };

    point position(const ray& r, float time)
    {
        return r.origin + r.direction * time;
    }

    // Applies a transformation matrix to a ray, returning a new ray.
    [[nodiscard]]
    rt::ray transformRay(const ray &r, const matrix<4> &m)
    {
        return {m * r.origin, m * r.direction};
    }

    /**
     * @brief Calculates the intersection points of a ray and a sphere.
     *
     * Transforms the ray into the sphere's local object space using the sphere's inverse
     * transformation matrix, then uses the quadratic formula to solve for the intersection times `t`.
     * Math reference: documentation/explanation/RaySphereIntersection.md
     *
     * @param s The target sphere geometry (includes scale/translation matrices).
     * @param r The ray being cast in world space.
     * @return std::vector<intersection> A collection of intersection records containing the `t` values and a pointer to the hit object. Empty if no intersection.
    */
    [[nodiscard]]
    std::vector<intersection> intersect(const sphere& s, const ray& r)
    {
        // 1. Transform the ray by the inverse of the sphere's matrix
        ray local_ray = transformRay(r, s.transform.inverse());

        // 2. Use local_ray for all subsequent math
        vector sphere_to_ray = local_ray.origin - s.origin;

        float a = dotProduct(local_ray.direction, local_ray.direction);
        float b = 2.0f * dotProduct(local_ray.direction, sphere_to_ray);
        float c = dotProduct(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius);

        float discriminant = (b * b) - (4 * a * c);

        if (discriminant < 0)
            return {}; // No intersection

        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        return { intersection(t1, &s), intersection(t2, &s) };
    }

    /**
     * @brief Identifies the closest valid intersection from a collection.
     *
     * Filters out intersections that occur behind the ray origin (t < 0) and returns the one with the smallest 't' value.
     *
     * @param intersectionSet A collection of intersection records to evaluate.
     * @return std::optional<intersection> The closest valid hit, or std::nullopt if no valid intersections exist.
    */
    [[nodiscard]]
    std::optional<intersection> hit(const std::vector<intersection>& intersectionSet)
    {
        auto valid_hits = intersectionSet | std::views::filter([](const auto& i) { return i.t >= 0.0f; });

        if (valid_hits.empty())
            return std::nullopt;

        return *std::ranges::min_element(valid_hits, {}, &intersection::t);
    }
}
