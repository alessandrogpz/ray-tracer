module;

#include <vector>
#include <optional>

export module rt.ray;

import rt.tuple;
import rt.intersection;
import rt.matrix;

export namespace rt {

    struct Ray {
        Point origin;
        Vector direction;

        Ray();
        Ray(Point _origin, Vector _direction);
    };

    [[nodiscard]] Point position(const Ray& r, double time);
    [[nodiscard]] Ray transformRay(const Ray &r, const Matrix<4> &m);
    [[nodiscard]] std::optional<Intersection> hit(const std::vector<Intersection>& intersectionSet);

} // namespace rt
