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
        Matrix<4> transform {identity()};
        Material material{};

        explicit Sphere(Point _origin, double _radius, Material _material = Material());
        Sphere();

        [[nodiscard]] bool operator==(const Sphere& other) const;
        void set_transform(const Matrix<4>& t);
    };

    [[nodiscard]] Vector normalAt(const Sphere& s, Point p);

} // namespace rt
