module;
#include <cmath>
module rt.sphere;

import std;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;
import rt.intersection;
import rt.ray;
import rt.shape_base;

namespace rt {

    Sphere::Sphere(Point _origin, double _radius, Material _material)
        : origin(_origin), radius(_radius)
    {
        material = _material;
    }

    Sphere::Sphere() : Sphere(Point(), 1.0, Material()) {}

    bool Sphere::operator==(const Sphere& other) const {
        return id == other.id;
    }

    Vector Sphere::local_normal_at(Point local_point) const {
        return local_point - origin;
    }

    LocalIntersections Sphere::local_intersect(Point local_origin, Vector local_direction) const {
        Vector sphere_to_ray = local_origin - origin;

        double a = dotProduct(local_direction, local_direction);
        double b = 2.0 * dotProduct(local_direction, sphere_to_ray);
        double c = dotProduct(sphere_to_ray, sphere_to_ray) - (radius * radius);

        double discriminant = (b * b) - (4.0 * a * c);

        if (discriminant < 0.0)
            return {};

        double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

        return {2, t1, t2};
    }

    std::vector<Intersection> intersect(const Shape& s, const Ray& r) {
        Ray local_ray = transformRay(r, s.get_transform_inverse());
        LocalIntersections ts = s.local_intersect(local_ray.origin, local_ray.direction);
        std::vector<Intersection> xs;
        xs.reserve(ts.count);
        if (ts.count > 0) xs.emplace_back(ts.t0, &s);
        if (ts.count > 1) xs.emplace_back(ts.t1, &s);
        return xs;
    }

    void intersect(const Shape& s, const Ray& r, std::vector<Intersection>& xs) {
        Ray local_ray = transformRay(r, s.get_transform_inverse());
        LocalIntersections ts = s.local_intersect(local_ray.origin, local_ray.direction);
        if (ts.count > 0) xs.emplace_back(ts.t0, &s);
        if (ts.count > 1) xs.emplace_back(ts.t1, &s);
    }

    Vector normalAt(const Shape& s, Point p) {
        const Point local_point = s.get_transform_inverse() * p;
        const Vector local_normal = s.local_normal_at(local_point);
        const Tuple world_normal_tuple = s.get_transform_inverse_transpose() * static_cast<const Tuple&>(local_normal);
        const Vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);
        return normalizeVector(world_normal);
    }

} // namespace rt
