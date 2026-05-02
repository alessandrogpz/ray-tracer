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
    matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    point p = createPoint(-3.0f, 4.0f, 5.0f);
    
    EXPECT_EQ(transform * p, point(2.0f, 1.0f, 7.0f));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    matrix<4> inv = transform.inverse();

    point p = createPoint(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(inv * p, point(-8.0f, 7.0f, 3.0f));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    vector p = vector(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(transform * p, p);
}

TEST(MatrixTransformations, ScaleMatrixByPoint)
{
    matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    point p = createPoint(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * p, point(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScaleMatrixByVector)
{
    matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    vector v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * v, vector(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScaleInverseMatrixByVector)
{
    matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    matrix<4> inv = transform.inverse();
    vector v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(inv * v, vector(-2.0f, 2.0f, 2.0f));
}

TEST(MatrixTransformations, ReflectingMatrixOnXAxis)
{
    matrix<4> transform = scale(-1.0f, 1.0f, 1.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);

    EXPECT_EQ(transform * p, point(-2.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, RotatingPointAroundXAxis)
{
    point p = createPoint(0.0f, 1.0f, 0.0f);
    matrix<4> half_quarter = rotation_x(static_cast<float>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_x(static_cast<float>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(0.0f, std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f));
    EXPECT_EQ(full_quarter * p, point(0.0f, 0.0f, 1.0f));
}

TEST(MatrixTransformations, RotatePointAroundXAxisInReverseUsingInverse)
{
    point p = createPoint(0.0f, 1.0f, 0.0f);
    matrix<4> half_quarter = rotation_x(static_cast<float>(std::numbers::pi / 4.0));
    matrix<4> inv = half_quarter.inverse();

    EXPECT_EQ(inv * p, point(0.0f, std::sqrt(2.0f) / 2.0f, - std::sqrt(2.0f) / 2.0f));
}

TEST(MatrixTransformations, RotatingPointAroundYAxis)
{
    point p = createPoint(0.0f, 0.0f, 1.0f);
    matrix<4> half_quarter = rotation_y(static_cast<float>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_y(static_cast<float>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(std::sqrt(2.0f) / 2.0f, 0.0f, std::sqrt(2.0f) / 2.0f));
    EXPECT_EQ(full_quarter * p, point(1.0f, 0.0f, 0.0f));
}

TEST(MatrixTransformations, RotatingPointAroundZAxis)
{
    point p = createPoint(0.0f, 1.0f, 0.0f);
    matrix<4> half_quarter = rotation_z(static_cast<float>(std::numbers::pi / 4.0));
    matrix<4> full_quarter = rotation_z(static_cast<float>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, point(-std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0.0f));
    EXPECT_EQ(full_quarter * p, point(-1.0f, 0.0f, 0.0f));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToY)
{
    matrix<4> transform = shear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(5.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToZ)
{
    matrix<4> transform = shear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(6.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToX)
{
    matrix<4> transform = shear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(2.0f, 5.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToZ)
{
    matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(2.0f, 7.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToX)
{
    matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(2.0f, 3.0f, 6.0f));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToY)
{
    matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, point(2.0f, 3.0f, 7.0f));
}

TEST(MatrixTransformations, TransformationsInSequnce)
{
    point p = createPoint(1.0f, 0.0f, 1.0f);
    matrix<4> A = rotation_x(std::numbers::pi / 2.0f);
    matrix<4> B = scale(5.0f, 5.0f, 5.0f);
    matrix<4> C = translation(10.0f, 5.0f, 7.0f);

    // Applying rotation
    point p2 = A * p;
    EXPECT_EQ(p2, point(1.0f, -1.0f, 0.0f));

    // Applying scaling
    point p3 = B * p2;
    EXPECT_EQ(p3, point(5.0f, -5.0f, 0.0f));

    // Applying translation
    point p4 = C * p3;
    EXPECT_EQ(p4, point(15.0f, 0.0f, 7.0f));

    // Chaining transformations
    point p5 = C * B * A * p;
    EXPECT_EQ(p5, point(15.0f, 0.0f, 7.0f));
}