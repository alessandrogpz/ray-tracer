#include <gtest/gtest.h>
#include "../includes/Matrix.hpp"
#include "../includes/Tuple.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Transformations.hpp"

// ---------------------------------------------------
// Matrix Transformation

TEST(MatrixTransformations, MultiplyPointByTranslationMatrix)
{
    matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    tuple p = createPoint(-3.0f, 4.0f, 5.0f);
    
    EXPECT_EQ(transform * p, createPoint(2.0f, 1.0f, 7.0f));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    matrix<4> tranform = translation(5.0f, -3.0f, 2.0f);
    matrix<4> inv = tranform.inverse();

    tuple p = createPoint(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(inv * p, createPoint(-8.0f, 7.0f, 3.0f));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    matrix<4> transform = translation(5.0f, -3.0f, 2.0f);
    tuple p = createVector(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(transform * p, p);
}

TEST(MatrixTransformations, ScalingMatrixByPoint)
{
    matrix<4> transform = scaling(2.0f, 3.0f, 4.0f);
    tuple p = createPoint(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * p, createPoint(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScalingMatrixByVector)
{
    matrix<4> transform = scaling(2.0f, 3.0f, 4.0f);
    tuple v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(transform * v, createVector(-8.0f, 18.0f, 32.0f));
}

TEST(MatrixTransformations, ScalingInverseMatrixByVector)
{
    matrix<4> transform = scaling(2.0f, 3.0f, 4.0f);
    matrix<4> inv = transform.inverse();
    tuple v = createVector(-4.0f, 6.0f, 8.0f);

    EXPECT_EQ(inv * v, createVector(-2.0f, 2.0f, 2.0f));
}

TEST(MatrixTransformations, ReflectingMatrixOnXAxis)
{
    matrix<4> transform = scaling(-1.0f, 1.0f, 1.0f);
    tuple p = createPoint(2.0f, 3.0f, 4.0f);

    EXPECT_EQ(transform * p, createPoint(-2.0f, 3.0f, 4.0f));
}