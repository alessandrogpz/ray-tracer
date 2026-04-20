#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "Matrix.hpp"

matrix<4> identity();
matrix<4> translation(float x, float y, float z);
matrix<4> scaling(float x, float y, float z);
matrix<4> rotation_x(float radians);
matrix<4> rotation_y(float radians);
matrix<4> rotation_z(float radians);
matrix<4> shear(float xy, float xz, float yx, float yz, float zx, float zy);

#endif