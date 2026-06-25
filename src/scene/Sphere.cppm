module;

#include <vector>

export module rt.sphere;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;
import rt.intersection;
import rt.shape_base;
import rt.ray;

export namespace rt {

    struct Sphere : public Shape {
        Point origin;
        double radius;

        explicit Sphere(Point _origin, double _radius, Material _material = Material());
        Sphere();

        [[nodiscard]] bool operator==(const Sphere& other) const;

        [[nodiscard]] Vector local_normal_at(Point local_point) const override;
        [[nodiscard]] std::vector<double> local_intersect(Point local_origin, Vector local_direction) const override;
    };

    [[nodiscard]] std::vector<Intersection> intersect(const Shape& s, const Ray& r);
    [[nodiscard]] Vector normalAt(const Shape& s, Point p);

} // namespace rt
