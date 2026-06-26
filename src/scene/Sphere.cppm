export module rt.sphere;

import std;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;
import rt.intersection;
import rt.ray;

export namespace rt {

    struct LocalIntersections {
        std::size_t count = 0;
        double t0 = 0.0;
        double t1 = 0.0;
    };

    struct Sphere {
        Point origin;
        double radius;
        Material material;
        Matrix<4> transform{identity()};
        Matrix<4> transform_inverse{identity()};
        Matrix<4> transform_inverse_transpose{identity()};
        int id;

        explicit Sphere(Point _origin, double _radius, Material _material = Material());
        Sphere();

        [[nodiscard]] bool operator==(const Sphere& other) const;

        void set_transform(const Matrix<4>& t);
        [[nodiscard]] const Matrix<4>& get_transform() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse_transpose() const;

        [[nodiscard]] Vector local_normal_at(Point local_point) const;
        [[nodiscard]] LocalIntersections local_intersect(Point local_origin, Vector local_direction) const;
    };

    [[nodiscard]] std::vector<Intersection> intersect(const Sphere& s, const Ray& r, std::uint32_t index = 0);
    void intersect(const Sphere& s, const Ray& r, std::vector<Intersection>& xs, std::uint32_t index = 0);
    [[nodiscard]] Vector normalAt(const Sphere& s, Point p);

} // namespace rt
