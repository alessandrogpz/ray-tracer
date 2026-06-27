#include <gtest/gtest.h>

import std;

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
    Matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    Point p = createPoint(-3.0f, 4.0f, 5.0f);
    
    EXPECT_EQ(transform * p, Point(2.0f, 1.0f, 7.0f));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    Matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    Matrix<4> inv = transform.inverse();

    Point p = createPoint(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(inv * p, Point(-8.0f, 7.0f, 3.0f));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    Matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    Vector p = Vector(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(transform * p, p);
}

TEST(MatrixTransformations, ScaleMatrixByPoint)
{
    Matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    Point p = createPoint(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * p, Point(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScaleMatrixByVector)
{
    Matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    Vector v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * v, Vector(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScaleInverseMatrixByVector)
{
    Matrix<4> transform = scale(2.0f, 3.0f, 4.0f);
    Matrix<4> inv = transform.inverse();
    Vector v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(inv * v, Vector(-2.0f, 2.0f, 2.0f));
}

TEST(MatrixTransformations, ReflectingMatrixOnXAxis)
{
    Matrix<4> transform = scale(-1.0f, 1.0f, 1.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);

    EXPECT_EQ(transform * p, Point(-2.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, RotatingPointAroundXAxis)
{
    Point p = createPoint(0.0f, 1.0f, 0.0f);
    Matrix<4> half_quarter = rotation_x(static_cast<float>(std::numbers::pi / 4.0f));
    Matrix<4> full_quarter = rotation_x(static_cast<float>(std::numbers::pi / 2.0f));

    EXPECT_EQ(half_quarter * p, Point(0.0f, std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f));
    EXPECT_EQ(full_quarter * p, Point(0.0f, 0.0f, 1.0f));
}

TEST(MatrixTransformations, RotatePointAroundXAxisInReverseUsingInverse)
{
    Point p = createPoint(0.0f, 1.0f, 0.0f);
    Matrix<4> half_quarter = rotation_x(static_cast<float>(std::numbers::pi / 4.0f));
    Matrix<4> inv = half_quarter.inverse();

    EXPECT_EQ(inv * p, Point(0.0f, std::sqrt(2.0f) / 2.0f, - std::sqrt(2.0f) / 2.0f));
}

TEST(MatrixTransformations, RotatingPointAroundYAxis)
{
    Point p = createPoint(0.0f, 0.0f, 1.0f);
    Matrix<4> half_quarter = rotation_y(static_cast<float>(std::numbers::pi / 4.0f));
    Matrix<4> full_quarter = rotation_y(static_cast<float>(std::numbers::pi / 2.0f));

    EXPECT_EQ(half_quarter * p, Point(std::sqrt(2.0f) / 2.0f, 0.0f, std::sqrt(2.0f) / 2.0f));
    EXPECT_EQ(full_quarter * p, Point(1.0f, 0.0f, 0.0f));
}

TEST(MatrixTransformations, RotatingPointAroundZAxis)
{
    Point p = createPoint(0.0f, 1.0f, 0.0f);
    Matrix<4> half_quarter = rotation_z(static_cast<float>(std::numbers::pi / 4.0f));
    Matrix<4> full_quarter = rotation_z(static_cast<float>(std::numbers::pi / 2.0f));

    EXPECT_EQ(half_quarter * p, Point(-std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0.0f));
    EXPECT_EQ(full_quarter * p, Point(-1.0f, 0.0f, 0.0f));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToY)
{
    Matrix<4> transform = shear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(5.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToZ)
{
    Matrix<4> transform = shear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(6.0f, 3.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToX)
{
    Matrix<4> transform = shear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(2.0f, 5.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToZ)
{
    Matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(2.0f, 7.0f, 4.0f));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToX)
{
    Matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(2.0f, 3.0f, 6.0f));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToY)
{
    Matrix<4> transform = shear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Point p = createPoint(2.0f, 3.0f, 4.0f);
    EXPECT_EQ(transform * p, Point(2.0f, 3.0f, 7.0f));
}

TEST(MatrixTransformations, TransformationsInSequnce)
{
    Point p = createPoint(1.0f, 0.0f, 1.0f);
    Matrix<4> A = rotation_x(std::numbers::pi / 2.0f);
    Matrix<4> B = scale(5.0f, 5.0f, 5.0f);
    Matrix<4> C = translation(10.0f, 5.0f, 7.0f);

    // Applying rotation
    Point p2 = A * p;
    EXPECT_EQ(p2, Point(1.0f, -1.0f, 0.0f));

    // Applying scaling
    Point p3 = B * p2;
    EXPECT_EQ(p3, Point(5.0f, -5.0f, 0.0f));

    // Applying translation
    Point p4 = C * p3;
    EXPECT_EQ(p4, Point(15.0f, 0.0f, 7.0f));

    // Chaining transformations
    Point p5 = C * B * A * p;
    EXPECT_EQ(p5, Point(15.0f, 0.0f, 7.0f));
}

TEST(MatrixTransformations, ReflectionMatrixCalculatesCorrectVector)
{
    // Scenario 1: Reflecting a Vector approaching at 45 degrees
    Vector v1 = createVector(1.0f, -1.0f, 0.0f);
    Vector n1 = createVector(0.0f, 1.0f, 0.0f);
    Matrix<4> R1 = reflection(n1);
    EXPECT_EQ(R1 * v1, Vector(1.0f, 1.0f, 0.0f));

    // Scenario 2: Reflecting a Vector off a slanted surface
    Vector v2 = createVector(0.0f, -1.0f, 0.0f);
    Vector n2 = createVector(std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0.0f);
    Matrix<4> R2 = reflection(n2);
    EXPECT_EQ(R2 * v2, Vector(1.0f, 0.0f, 0.0f));
}