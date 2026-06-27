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

namespace rt {

    Sphere::Sphere(Point _origin, float _radius, Material _material)
        : origin(_origin), radius(_radius), material(_material)
    {}

    Sphere::Sphere() : Sphere(Point(), 1.0f, Material()) {}

    bool Sphere::operator==(const Sphere& other) const {
        return origin == other.origin &&
               radius == other.radius &&
               material == other.material &&
               transform == other.transform;
    }

    void Sphere::set_transform(const Matrix<4>& t) {
        transform = t;
        transform_inverse = t.inverse();
        transform_inverse_transpose = transform_inverse.transpose();
    }

    const Matrix<4>& Sphere::get_transform() const {
        return transform;
    }

    const Matrix<4>& Sphere::get_transform_inverse() const {
        return transform_inverse;
    }

    const Matrix<4>& Sphere::get_transform_inverse_transpose() const {
        return transform_inverse_transpose;
    }

    Vector Sphere::local_normal_at(Point local_point) const {
        return local_point - origin;
    }

    LocalIntersections Sphere::local_intersect(Point local_origin, Vector local_direction) const {
        Vector sphere_to_ray = local_origin - origin;

        float a = dotProduct(local_direction, local_direction);
        float b = 2.0f * dotProduct(local_direction, sphere_to_ray);
        float c = dotProduct(sphere_to_ray, sphere_to_ray) - (radius * radius);

        float discriminant = (b * b) - (4.0f * a * c);

        if (discriminant < 0.0f)
            return {};

        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        return {2, t1, t2};
    }

    std::vector<Intersection> intersect(const Sphere& s, const Ray& r, std::uint32_t index) {
        Ray local_ray = transformRay(r, s.get_transform_inverse());
        LocalIntersections ts = s.local_intersect(local_ray.origin, local_ray.direction);
        std::vector<Intersection> xs;
        xs.reserve(ts.count);
        if (ts.count > 0) xs.emplace_back(ts.t0, index, ShapeType::Sphere);
        if (ts.count > 1) xs.emplace_back(ts.t1, index, ShapeType::Sphere);
        return xs;
    }

    void intersect(const Sphere& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index) {
        Ray local_ray = transformRay(r, s.get_transform_inverse());
        LocalIntersections ts = s.local_intersect(local_ray.origin, local_ray.direction);
        if (ts.count > 0) xs.emplace_back(ts.t0, index, ShapeType::Sphere);
        if (ts.count > 1) xs.emplace_back(ts.t1, index, ShapeType::Sphere);
    }

    Vector normalAt(const Sphere& s, const Point& p) {
        const Point local_point = s.get_transform_inverse() * p;
        const Vector local_normal = s.local_normal_at(local_point);
        const Tuple world_normal_tuple = s.get_transform_inverse_transpose() * static_cast<const Tuple&>(local_normal);
        const Vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);
        return normalizeVector(world_normal);
    }

} // namespace rt
