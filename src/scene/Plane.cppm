export module rt.plane;

import std;

import rt.matrix;
import rt.materials;
import rt.ray;
import rt.tuple;
import rt.intersection;
import rt.transformations;

export namespace rt {

    struct Plane {
        Material material = Material();
        Matrix<4> transform{identity()};
        Matrix<4> transform_inverse{identity()};
        Matrix<4> transform_inverse_transpose{identity()};

        Plane() = default;

        // Member function to calculate local intersection with the y=0 plane
        [[nodiscard]] std::optional<float> local_intersect(Point local_origin, Vector local_direction) const;

        // Member function to calculate local normal (always returns Vector(0, 1, 0))
        [[nodiscard]] Vector local_normal_at(Point local_point) const;

        void set_transform(const Matrix<4>& t);
        [[nodiscard]] const Matrix<4>& get_transform() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse_transpose() const;
    };

    // Free functions for world-space calculations matching Sphere's signature
    [[nodiscard]] std::vector<Intersection> intersect(const Plane& p, const Ray& r, std::uint32_t index = 0);
    void intersect(const Plane& p, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0);
    
    [[nodiscard]] Vector normalAt(const Plane& p, const Point& p_world);

} // namespace rt