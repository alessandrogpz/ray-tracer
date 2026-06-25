module rt.ray;

import std;

import rt.tuple;
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



    std::optional<Intersection> hit(const std::vector<Intersection>& intersectionSet) {
        auto valid_hits = intersectionSet | std::views::filter([](const auto& i) { return i.t >= 0.0; });

        if (valid_hits.empty())
            return std::nullopt;

        return *std::ranges::min_element(valid_hits, {}, &Intersection::t);
    }

} // namespace rt
