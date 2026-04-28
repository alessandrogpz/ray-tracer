#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "Matrix.hpp"

[[nodiscard]] matrix<4> identity();
[[nodiscard]] matrix<4> translation(float x, float y, float z);
[[nodiscard]] matrix<4> scale(float x, float y, float z);
[[nodiscard]] matrix<4> rotation_x(float radians);
[[nodiscard]] matrix<4> rotation_y(float radians);
[[nodiscard]] matrix<4> rotation_z(float radians);
[[nodiscard]] matrix<4> shear(float xy, float xz, float yx, float yz, float zx, float zy);

#endif