export module rt.transformations;

import rt.matrix;
import rt.tuple;

export namespace rt {

    [[nodiscard]] Matrix<4> identity();
    [[nodiscard]] Matrix<4> translation(float x, float y, float z);
    [[nodiscard]] Matrix<4> scale(float x, float y, float z);
    [[nodiscard]] Matrix<4> rotation_x(float radians);
    [[nodiscard]] Matrix<4> rotation_y(float radians);
    [[nodiscard]] Matrix<4> rotation_z(float radians);
    [[nodiscard]] Matrix<4> shear(float xy, float xz, float yx, float yz, float zx, float zy);
    [[nodiscard]] Matrix<4> reflection(const Vector& normal);

} // namespace rt
