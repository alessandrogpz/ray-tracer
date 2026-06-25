export module rt.shapes;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;

export namespace rt {

    // TODO: When introducing the "World" scene container and adding new shape primitives,
    // implement Data-Oriented Design (DOD) memory layouts:
    // 1. Store shapes contiguously in standard vectors (e.g. std::vector<Sphere>, std::vector<Plane>)
    //    inside the World object to maximize CPU cache line prefetching.
    // 2. Refactor Intersection to use a lightweight index/ID instead of concrete shape pointers
    //    to eliminate pointer chasing and cache misses.
    struct Sphere {
        int id;
        Point origin;
        double radius;
        Material material{};

        explicit Sphere(Point _origin, double _radius, Material _material = Material());
        Sphere();

        [[nodiscard]] bool operator==(const Sphere& other) const;
        void set_transform(const Matrix<4>& t);
        [[nodiscard]] const Matrix<4>& get_transform() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse_transpose() const;

    private:
        Matrix<4> transform {identity()};
        Matrix<4> transform_inverse {identity()};
        Matrix<4> transform_inverse_transpose {identity()};
    };

    [[nodiscard]] Vector normalAt(const Sphere& s, Point p);

} // namespace rt
