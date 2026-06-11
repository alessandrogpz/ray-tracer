module;

#include <cmath>

export module rt.transformations;

import rt.matrix;

export namespace rt {

    [[nodiscard]]
    matrix<4> identity() {
        matrix<4> I; // Default all components to 0.0

        I(0, 0) = 1.0;
        I(1, 1) = 1.0;
        I(2, 2) = 1.0;
        I(3, 3) = 1.0;

        return I;
    }

    [[nodiscard]]
    matrix<4> translation(double x, double y, double z) {

        matrix<4> transform = identity();

        transform(0, 3) = x;
        transform(1, 3) = y;
        transform(2, 3) = z;

        return transform;
    }

    [[nodiscard]]
    matrix<4> scale(double x, double y, double z) {
        
        matrix<4> scale_mat = identity();

        scale_mat(0, 0) = x;
        scale_mat(1, 1) = y;
        scale_mat(2, 2) = z;

        return scale_mat;
    }

    [[nodiscard]]
    matrix<4> rotation_x(double radians) {
        
        matrix<4> transform = identity();
        
        transform(1, 1) = std::cos(radians);
        transform(1, 2) = -std::sin(radians);
        transform(2, 1) = std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    [[nodiscard]]
    matrix<4> rotation_y(double radians) {
        
        matrix<4> transform = identity();
        
        transform(0, 0) = std::cos(radians);
        transform(0, 2) = std::sin(radians);
        transform(2, 0) = -std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    [[nodiscard]]
    matrix<4> rotation_z(double radians) {
        
        matrix<4> transform = identity();
        
        transform(0, 0) = std::cos(radians);
        transform(0, 1) = -std::sin(radians);
        transform(1, 0) = std::sin(radians);
        transform(1, 1) = std::cos(radians);

        return transform;
    }

    [[nodiscard]]
    matrix<4> shear(double xy, double xz, double yx, double yz, double zx, double zy) {

        matrix<4> transform = identity();

        transform(0, 1) = xy;
        transform(0, 2) = xz;
        transform(1, 0) = yx;
        transform(1, 2) = yz;
        transform(2, 0) = zx;
        transform(2, 1) = zy;

        return transform;
    }

} // namespace rt
