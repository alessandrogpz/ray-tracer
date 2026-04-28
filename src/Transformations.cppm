module;

#include <cmath>

export module rt.transformations;

import rt.matrix;

export namespace rt {

    [[nodiscard]] matrix<4> identity() {
        matrix<4> I; // Default all components to 0.0f

        I(0, 0) = 1.0f;
        I(1, 1) = 1.0f;
        I(2, 2) = 1.0f;
        I(3, 3) = 1.0f;

        return I;
    }

    [[nodiscard]] matrix<4> translation(float x, float y, float z) {

        matrix<4> transform = identity();

        transform(0, 3) = x;
        transform(1, 3) = y;
        transform(2, 3) = z;

        return transform;
    }

    [[nodiscard]] matrix<4> scale(float x, float y, float z) {
        
        matrix<4> scale_mat = identity();

        scale_mat(0, 0) = x;
        scale_mat(1, 1) = y;
        scale_mat(2, 2) = z;

        return scale_mat;
    }

    [[nodiscard]] matrix<4> rotation_x(float radians) {
        
        matrix<4> transform = identity();
        
        transform(1, 1) = std::cos(radians);
        transform(1, 2) = -std::sin(radians);
        transform(2, 1) = std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    [[nodiscard]] matrix<4> rotation_y(float radians) {
        
        matrix<4> transform = identity();
        
        transform(0, 0) = std::cos(radians);
        transform(0, 2) = std::sin(radians);
        transform(2, 0) = -std::sin(radians);
        transform(2, 2) = std::cos(radians);

        return transform;
    }

    [[nodiscard]] matrix<4> rotation_z(float radians) {
        
        matrix<4> transform = identity();
        
        transform(0, 0) = std::cos(radians);
        transform(0, 1) = -std::sin(radians);
        transform(1, 0) = std::sin(radians);
        transform(1, 1) = std::cos(radians);

        return transform;
    }

    [[nodiscard]] matrix<4> shear(float xy, float xz, float yx, float yz, float zx, float zy) {

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
