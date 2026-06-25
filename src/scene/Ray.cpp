module;

#include <vector>
#include <cmath>
#include <ranges>
#include <algorithm>
#include <optional>

module rt.ray;

import rt.tuple;
import rt.shapes;
import rt.intersection;
import rt.matrix;
import rt.transformations;

namespace rt {

    Ray::Ray() : origin(createPoint(0.0, 0.0, 0.0)),
                 direction(createVector(0.0, 0.0, 0.0)) {}

    Ray::Ray(Point _origin, Vector _direction)
        : origin(_origin), direction(_direction) {}

    Point position(const Ray& r, double time) {
        return r.origin + r.direction * time;
    }

    Ray transformRay(const Ray &r, const Matrix<4> &m) {
        return {m * r.origin, m * r.direction};
    }

    std::vector<Intersection> intersect(const Sphere& s, const Ray& r) {
        Ray local_ray = transformRay(r, s.transform.inverse());
        Vector sphere_to_ray = local_ray.origin - s.origin;

        double a = dotProduct(local_ray.direction, local_ray.direction);
        double b = 2.0 * dotProduct(local_ray.direction, sphere_to_ray);
        double c = dotProduct(sphere_to_ray, sphere_to_ray) - (s.radius * s.radius);

        double discriminant = (b * b) - (4.0 * a * c);

        if (discriminant < 0.0)
            return {};

        double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        return { Intersection(t1, &s), Intersection(t2, &s) };
    }

    std::optional<Intersection> hit(const std::vector<Intersection>& intersectionSet) {
        auto valid_hits = intersectionSet | std::views::filter([](const auto& i) { return i.t >= 0.0; });

        if (valid_hits.empty())
            return std::nullopt;

        return *std::ranges::min_element(valid_hits, {}, &Intersection::t);
    }

} // namespace rt
