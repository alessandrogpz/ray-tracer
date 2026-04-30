module;

#include <vector>
#include <cmath>

export module rt.ray;

import rt.tuple;
import rt.shapes;

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

    [[nodiscard]]
    point position(const ray& r, float time)
    {
        return r.origin + r.direction * time;
    }

    /**
    * @brief Calculates the intersection points of a ray and a sphere.
    *
    * Uses the quadratic formula to solve for time `t` where the ray intersects the sphere's surface.
    * Math reference: documentation/explanation/RaySphereIntersection.md
    *
    * @param s The target sphere geometry.
    * @param r The ray being cast.
    *  @return std::vector<float> A collection of `t` values representing intersection distances. Empty if no intersection.
    */
    [[nodiscard]]
    std::vector<float> intersect(const sphere& s, const ray& r)
    {
        vector sphere_to_ray = r.origin - s.origin;

        float a = dotProduct(r.direction, r.direction);
        float b = 2.0f * dotProduct(r.direction, sphere_to_ray);
        float c = dotProduct(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius);

        float discriminant = (b * b) - (4 * a * c);

        if (discriminant < 0)
            return {}; // No intersection

        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        return {t1, t2};
    }
}
