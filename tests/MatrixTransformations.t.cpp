#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

// ---------------------------------------------------
// Matrix Transformation

TEST(MatrixTransformations, MultiplyPointByTranslationMatrix)
{
    matrix<4> transform = translation(5.0, -3.0, 2.0);
    point p = createPoint(-3.0, 4.0, 5.0);
    
    EXPECT_EQ(transform * p, point(2.0, 1.0, 7.0));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    matrix<4> transform = translation(5.0, -3.0, 2.0);
    matrix<4> inv = transform.inverse();

    point p = createPoint(-3.0, 4.0, 5.0);

    EXPECT_EQ(inv * p, point(-8.0, 7.0, 3.0));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    matrix<4> transform = translation(5.0, -3.0, 2.0);
    vector p = vector(-3.0, 4.0, 5.0);

    EXPECT_EQ(transform * p, p);
}

TEST(MatrixTransformations, ScaleMatrixByPoint)
{
    matrix<4> transform = scale(2.0, 3.0, 4.0);
    point p = createPoint(-4.0, 6.0, 8.0);

    EXPECT_EQ(transform * p, point(-8.0, 18.0, 32.0));
}

TEST(MatrixTransformations, ScaleMatrixByVector)
{
    matrix<4> transform = scale(2.0, 3.0, 4.0);
    vector v = createVector(-4.0, 6.0, 8.0);

    EXPECT_EQ(transform * v, vector(-8.0, 18.0, 32.0));
}

TEST(MatrixTransformations, ScaleInverseMatrixByVector)
{
    matrix<4> transform = scale(2.0, 3.0, 4.0);
    matrix<4> inv = transform.inverse();
    vector v = createVector(-4.0, 6.0, 8.0);

    EXPECT_EQ(inv * v, vector(-2.0, 2.0, 2.0));
}

TEST(MatrixTransformations, ReflectingMatrixOnXAxis)
{
    matrix<4> transform = scale(-1.0, 1.0, 1.0);
    point p = createPoint(2.0, 3.0, 4.0);

    EXPECT_EQ(transform * p, point(-2.0, 3.0, 4.0));
}

TEST(MatrixTransformations, RotatingPointAroundXAxis)
{
    point p = createPoint(0.0, 1.0, 0.0);
    matrix<4> half_quarter = rotation_x(static_cast<double>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_x(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(0.0, std::sqrt(2.0) / 2.0, std::sqrt(2.0) / 2.0));
    EXPECT_EQ(full_quarter * p, point(0.0, 0.0, 1.0));
}

TEST(MatrixTransformations, RotatePointAroundXAxisInReverseUsingInverse)
{
    point p = createPoint(0.0, 1.0, 0.0);
    matrix<4> half_quarter = rotation_x(static_cast<double>(std::numbers::pi / 4.0));
    matrix<4> inv = half_quarter.inverse();

    EXPECT_EQ(inv * p, point(0.0, std::sqrt(2.0) / 2.0, - std::sqrt(2.0) / 2.0));
}

TEST(MatrixTransformations, RotatingPointAroundYAxis)
{
    point p = createPoint(0.0, 0.0, 1.0);
    matrix<4> half_quarter = rotation_y(static_cast<double>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_y(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(std::sqrt(2.0) / 2.0, 0.0, std::sqrt(2.0) / 2.0));
    EXPECT_EQ(full_quarter * p, point(1.0, 0.0, 0.0));
}

TEST(MatrixTransformations, RotatingPointAroundZAxis)
{
    point p = createPoint(0.0, 1.0, 0.0);
    matrix<4> half_quarter = rotation_z(static_cast<double>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_z(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(-std::sqrt(2.0) / 2.0, std::sqrt(2.0) / 2.0, 0.0));
    EXPECT_EQ(full_quarter * p, point(-1.0, 0.0, 0.0));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToY)
{
    matrix<4> transform = shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(5.0, 3.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToZ)
{
    matrix<4> transform = shear(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(6.0, 3.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToX)
{
    matrix<4> transform = shear(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 5.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToZ)
{
    matrix<4> transform = shear(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 7.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToX)
{
    matrix<4> transform = shear(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 3.0, 6.0));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToY)
{
    matrix<4> transform = shear(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 3.0, 7.0));
}

TEST(MatrixTransformations, TransformationsInSequnce)
{
    point p = createPoint(1.0, 0.0, 1.0);
    matrix<4> A = rotation_x(std::numbers::pi / 2.0);
    matrix<4> B = scale(5.0, 5.0, 5.0);
    matrix<4> C = translation(10.0, 5.0, 7.0);

    // Applying rotation
    point p2 = A * p;
    EXPECT_EQ(p2, point(1.0, -1.0, 0.0));

    // Applying scaling
    point p3 = B * p2;
    EXPECT_EQ(p3, point(5.0, -5.0, 0.0));

    // Applying translation
    point p4 = C * p3;
    EXPECT_EQ(p4, point(15.0, 0.0, 7.0));

    // Chaining transformations
    point p5 = C * B * A * p;
    EXPECT_EQ(p5, point(15.0, 0.0, 7.0));
}