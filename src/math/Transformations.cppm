export module rt.transformations;

import rt.matrix;
import rt.tuple;

export namespace rt {

    [[nodiscard]] Matrix<4> identity();
    [[nodiscard]] Matrix<4> translation(double x, double y, double z);
    [[nodiscard]] Matrix<4> scale(double x, double y, double z);
    [[nodiscard]] Matrix<4> rotation_x(double radians);
    [[nodiscard]] Matrix<4> rotation_y(double radians);
    [[nodiscard]] Matrix<4> rotation_z(double radians);
    [[nodiscard]] Matrix<4> shear(double xy, double xz, double yx, double yz, double zx, double zy);
    [[nodiscard]] Matrix<4> reflection(Vector normal);

} // namespace rt
