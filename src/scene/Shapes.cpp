module rt.shapes;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;

namespace rt {

    Sphere::Sphere(Point _origin, double _radius, Material _material)
        : origin(_origin), radius(_radius), material(_material)
    {
        static int next_id = 0;
        id = ++next_id;
    }

    Sphere::Sphere() : Sphere(Point(), 1.0, Material()) {}

    bool Sphere::operator==(const Sphere& other) const {
        return id == other.id;
    }

    void Sphere::set_transform(const Matrix<4>& t) {
        transform = t;

        // Cache in Object
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

    Vector normalAt(const Sphere& s, Point p) {
        const Point object_point = s.get_transform_inverse() * p;
        const Vector object_normal = object_point - createPoint(0.0, 0.0, 0.0);
        const Tuple world_normal_tuple = s.get_transform_inverse_transpose() * static_cast<const Tuple&>(object_normal);
        const Vector world_normal = createVector(world_normal_tuple.x, world_normal_tuple.y, world_normal_tuple.z);
        return normalizeVector(world_normal);
    }

} // namespace rt
