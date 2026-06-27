module rt.transformations;

import std;

import rt.matrix;
import rt.tuple;

namespace rt {

    Matrix<4> identity() {
        Matrix<4> I; // Default all components to 0.0

        I(0, 0) = 1.0;
        I(1, 1) = 1.0;
        I(2, 2) = 1.0;
        I(3, 3) = 1.0;

        return I;
    }

    Matrix<4> translation(double x, double y, double z) {
        Matrix<4> transform = identity();

        transform(0, 3) = x;
        transform(1, 3) = y;
        transform(2, 3) = z;

        return transform;
    }

    Matrix<4> scale(double x, double y, double z) {
        Matrix<4> scale_mat = identity();

        scale_mat(0, 0) = x;
        scale_mat(1, 1) = y;
        scale_mat(2, 2) = z;

        return scale_mat;
    }

    Matrix<4> rotation_x(double radians) {
        Matrix<4> transform = identity();

        transform(1, 1) = std::cos(radians);
        transform(1, 2) = -std::sin(radians);
        transform(2, 1) = std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    Matrix<4> rotation_y(double radians) {
        Matrix<4> transform = identity();

        transform(0, 0) = std::cos(radians);
        transform(0, 2) = std::sin(radians);
        transform(2, 0) = -std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    Matrix<4> rotation_z(double radians) {
        Matrix<4> transform = identity();

        transform(0, 0) = std::cos(radians);
        transform(0, 1) = -std::sin(radians);
        transform(1, 0) = std::sin(radians);
        transform(1, 1) = std::cos(radians);

        return transform;
    }

    Matrix<4> shear(double xy, double xz, double yx, double yz, double zx, double zy) {
        Matrix<4> transform = identity();

        transform(0, 1) = xy;
        transform(0, 2) = xz;
        transform(1, 0) = yx;
        transform(1, 2) = yz;
        transform(2, 0) = zx;
        transform(2, 1) = zy;

        return transform;
    }

    Matrix<4> reflection(const Vector& normal) {
        Matrix<4> outer_product{};

        double x = normal.x;
        double y = normal.y;
        double z = normal.z;
        double w = normal.w;

        outer_product(0, 0) = x * x;
        outer_product(0, 1) = x * y;
        outer_product(0, 2) = x * z;
        outer_product(0, 3) = x * w;

        outer_product(1, 0) = y * x;
        outer_product(1, 1) = y * y;
        outer_product(1, 2) = y * z;
        outer_product(1, 3) = y * w;

        outer_product(2, 0) = z * x;
        outer_product(2, 1) = z * y;
        outer_product(2, 2) = z * z;
        outer_product(2, 3) = z * w;

        outer_product(3, 0) = w * x;
        outer_product(3, 1) = w * y;
        outer_product(3, 2) = w * z;
        outer_product(3, 3) = w * w;

        return identity() - outer_product * 2;
    }

} // namespace rt
