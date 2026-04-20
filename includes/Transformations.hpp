#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

#include "Matrix.hpp"

matrix<4> identity();
matrix<4> translation(float x, float y, float z);
matrix<4> scaling(float x, float y, float z);

#endif