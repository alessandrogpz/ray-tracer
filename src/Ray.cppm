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
    struct Ray
    {
        Point origin;
        Vector direction;

        Ray() : origin(createPoint(0.0, 0.0, 0.0)),
                direction(createVector(0.0, 0.0, 0.0)) {}

        Ray(Point _origin, Vector _direction)
            : origin(_origin), direction(_direction) {}
    };

    Point position(const Ray& r, double time)
    {
        return r.origin + r.direction * time;
    }

    // Applies a transformation Matrix to a Ray, returning a new Ray.
    [[nodiscard]]
    rt::Ray transformRay(const Ray &r, const Matrix<4> &m)
    {
        return {m * r.origin, m * r.direction};
    }

    /**
     * @brief Calculates the Intersection points of a Ray and a Sphere.
     *
     * Transforms the Ray into the Sphere's local object space using the Sphere's inverse
     * transformation Matrix, then uses the quadratic formula to solve for the Intersection times `t`.
     * Math reference: documentation/explanation/RaySphereIntersection.md
     *
     * @param s The target Sphere geometry (includes scale/translation matrices).
     * @param r The Ray being cast in world space.
     * @return std::vector<Intersection> A collection of Intersection records containing the `t` values and a pointer to the hit object. Empty if no Intersection.
    */
    [[nodiscard]]
    std::vector<Intersection> intersect(const Sphere& s, const Ray& r)
    {
        // 1. Transform the Ray by the inverse of the Sphere's Matrix
        Ray local_ray = transformRay(r, s.transform.inverse());

        // 2. Use local_ray for all subsequent math
        Vector sphere_to_ray = local_ray.origin - s.origin;

        double a = dotProduct(local_ray.direction, local_ray.direction);
        double b = 2.0 * dotProduct(local_ray.direction, sphere_to_ray);
        double c = dotProduct(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius);

        double discriminant = (b * b) - (4 * a * c);

        if (discriminant < 0)
            return {}; // No Intersection

        double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        return { Intersection(t1, &s), Intersection(t2, &s) };
    }

    /**
     * @brief Identifies the closest valid Intersection from a collection.
     *
     * Filters out intersections that occur behind the Ray origin (t < 0) and returns the one with the smallest 't' value.
     *
     * @param intersectionSet A collection of Intersection records to evaluate.
     * @return std::optional<Intersection> The closest valid hit, or std::nullopt if no valid intersections exist.
    */
    [[nodiscard]]
    std::optional<Intersection> hit(const std::vector<Intersection>& intersectionSet)
    {
        auto valid_hits = intersectionSet | std::views::filter([](const auto& i) { return i.t >= 0.0; });

        if (valid_hits.empty())
            return std::nullopt;

        return *std::ranges::min_element(valid_hits, {}, &Intersection::t);
    }
}
