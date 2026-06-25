export module rt.shapes;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;

export namespace rt {

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
