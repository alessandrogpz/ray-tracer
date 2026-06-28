module rt.plane;

import rt.utils;

namespace rt {

    void Plane::set_transform(const Matrix<4>& t) {
        transform = t;
        transform_inverse = t.inverse();
        transform_inverse_transpose = transform_inverse.transpose();
    }

    const Matrix<4>& Plane::get_transform() const {
        return transform;
    }

    const Matrix<4>& Plane::get_transform_inverse() const {
        return transform_inverse;
    }

    const Matrix<4>& Plane::get_transform_inverse_transpose() const {
        return transform_inverse_transpose;
    }

    std::optional<float> Plane::local_intersect(Point local_origin, Vector local_direction) const {
        // If the ray is parallel to the plane (has no y-velocity/direction)
        if (std::abs(local_direction.y) < EPSILON) {
            return std::nullopt;
        }

        // Return distance t = -O_y / D_y
        return -local_origin.y / local_direction.y;
    }

    Vector Plane::local_normal_at(Point) const {
        // For a plane at y=0, the normal vector always points straight up (0, 1, 0)
        return createVector(0.0f, 1.0f, 0.0f);
    }

    std::vector<Intersection> intersect(const Plane& p, const Ray& r, std::uint32_t index) {
        Ray local_ray = transformRay(r, p.get_transform_inverse());
        auto t = p.local_intersect(local_ray.origin, local_ray.direction);
        if (t) {
            return { Intersection(*t, index, ShapeType::Plane) };
        }
        return {};
    }

    void intersect(const Plane& p, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index) {
        Ray local_ray = transformRay(r, p.get_transform_inverse());
        auto t = p.local_intersect(local_ray.origin, local_ray.direction);
        if (t) {
            xs.emplace_back(*t, index, ShapeType::Plane);
        }
    }

    Vector normalAt(const Plane& p, const Point& p_world) {
        const Point local_point = p.get_transform_inverse() * p_world;
        const Vector local_normal = p.local_normal_at(local_point);
        const Tuple world_normal_tuple = p.get_transform_inverse_transpose() * static_cast<const Tuple&>(local_normal);
        const Vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);
        return normalizeVector(world_normal);
    }

} // namespace rt
