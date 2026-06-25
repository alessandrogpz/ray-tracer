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
    Matrix<4> transform = translation(5.0, -3.0, 2.0);
    Point p = createPoint(-3.0, 4.0, 5.0);
    
    EXPECT_EQ(transform * p, Point(2.0, 1.0, 7.0));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    Matrix<4> transform = translation(5.0, -3.0, 2.0);
    Matrix<4> inv = transform.inverse();

    Point p = createPoint(-3.0, 4.0, 5.0);

    EXPECT_EQ(inv * p, Point(-8.0, 7.0, 3.0));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    Matrix<4> transform = translation(5.0, -3.0, 2.0);
    Vector p = Vector(-3.0, 4.0, 5.0);

    EXPECT_EQ(transform * p, p);
}

TEST(MatrixTransformations, ScaleMatrixByPoint)
{
    Matrix<4> transform = scale(2.0, 3.0, 4.0);
    Point p = createPoint(-4.0, 6.0, 8.0);

    EXPECT_EQ(transform * p, Point(-8.0, 18.0, 32.0));
}

TEST(MatrixTransformations, ScaleMatrixByVector)
{
    Matrix<4> transform = scale(2.0, 3.0, 4.0);
    Vector v = createVector(-4.0, 6.0, 8.0);

    EXPECT_EQ(transform * v, Vector(-8.0, 18.0, 32.0));
}

TEST(MatrixTransformations, ScaleInverseMatrixByVector)
{
    Matrix<4> transform = scale(2.0, 3.0, 4.0);
    Matrix<4> inv = transform.inverse();
    Vector v = createVector(-4.0, 6.0, 8.0);

    EXPECT_EQ(inv * v, Vector(-2.0, 2.0, 2.0));
}

TEST(MatrixTransformations, ReflectingMatrixOnXAxis)
{
    Matrix<4> transform = scale(-1.0, 1.0, 1.0);
    Point p = createPoint(2.0, 3.0, 4.0);

    EXPECT_EQ(transform * p, Point(-2.0, 3.0, 4.0));
}

TEST(MatrixTransformations, RotatingPointAroundXAxis)
{
    Point p = createPoint(0.0, 1.0, 0.0);
    Matrix<4> half_quarter = rotation_x(static_cast<double>(std::numbers::pi / 4.0));
    Matrix<4> full_quarter = rotation_x(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, Point(0.0, std::sqrt(2.0) / 2.0, std::sqrt(2.0) / 2.0));
    EXPECT_EQ(full_quarter * p, Point(0.0, 0.0, 1.0));
}

TEST(MatrixTransformations, RotatePointAroundXAxisInReverseUsingInverse)
{
    Point p = createPoint(0.0, 1.0, 0.0);
    Matrix<4> half_quarter = rotation_x(static_cast<double>(std::numbers::pi / 4.0));
    Matrix<4> inv = half_quarter.inverse();

    EXPECT_EQ(inv * p, Point(0.0, std::sqrt(2.0) / 2.0, - std::sqrt(2.0) / 2.0));
}

TEST(MatrixTransformations, RotatingPointAroundYAxis)
{
    Point p = createPoint(0.0, 0.0, 1.0);
    Matrix<4> half_quarter = rotation_y(static_cast<double>(std::numbers::pi / 4.0));
    Matrix<4> full_quarter = rotation_y(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, Point(std::sqrt(2.0) / 2.0, 0.0, std::sqrt(2.0) / 2.0));
    EXPECT_EQ(full_quarter * p, Point(1.0, 0.0, 0.0));
}

TEST(MatrixTransformations, RotatingPointAroundZAxis)
{
    Point p = createPoint(0.0, 1.0, 0.0);
    Matrix<4> half_quarter = rotation_z(static_cast<double>(std::numbers::pi / 4.0));
    Matrix<4> full_quarter = rotation_z(static_cast<double>(std::numbers::pi / 2.0));

    EXPECT_EQ(half_quarter * p, Point(-std::sqrt(2.0) / 2.0, std::sqrt(2.0) / 2.0, 0.0));
    EXPECT_EQ(full_quarter * p, Point(-1.0, 0.0, 0.0));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToY)
{
    Matrix<4> transform = shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(5.0, 3.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesXInProportionToZ)
{
    Matrix<4> transform = shear(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(6.0, 3.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToX)
{
    Matrix<4> transform = shear(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(2.0, 5.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesYInProportionToZ)
{
    Matrix<4> transform = shear(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(2.0, 7.0, 4.0));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToX)
{
    Matrix<4> transform = shear(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(2.0, 3.0, 6.0));
}

TEST(MatrixTransformations, ShearingMovesZInProportionToY)
{
    Matrix<4> transform = shear(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    Point p = createPoint(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, Point(2.0, 3.0, 7.0));
}

TEST(MatrixTransformations, TransformationsInSequnce)
{
    Point p = createPoint(1.0, 0.0, 1.0);
    Matrix<4> A = rotation_x(std::numbers::pi / 2.0);
    Matrix<4> B = scale(5.0, 5.0, 5.0);
    Matrix<4> C = translation(10.0, 5.0, 7.0);

    // Applying rotation
    Point p2 = A * p;
    EXPECT_EQ(p2, Point(1.0, -1.0, 0.0));

    // Applying scaling
    Point p3 = B * p2;
    EXPECT_EQ(p3, Point(5.0, -5.0, 0.0));

    // Applying translation
    Point p4 = C * p3;
    EXPECT_EQ(p4, Point(15.0, 0.0, 7.0));

    // Chaining transformations
    Point p5 = C * B * A * p;
    EXPECT_EQ(p5, Point(15.0, 0.0, 7.0));
}

TEST(MatrixTransformations, ReflectionMatrixCalculatesCorrectVector)
{
    // Scenario 1: Reflecting a Vector approaching at 45 degrees
    Vector v1 = createVector(1.0, -1.0, 0.0);
    Vector n1 = createVector(0.0, 1.0, 0.0);
    Matrix<4> R1 = reflection(n1);
    EXPECT_EQ(R1 * v1, Vector(1.0, 1.0, 0.0));

    // Scenario 2: Reflecting a Vector off a slanted surface
    Vector v2 = createVector(0.0, -1.0, 0.0);
    Vector n2 = createVector(std::sqrt(2.0) / 2.0, std::sqrt(2.0) / 2.0, 0.0);
    Matrix<4> R2 = reflection(n2);
    EXPECT_EQ(R2 * v2, Vector(1.0, 0.0, 0.0));
}