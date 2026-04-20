#include <gtest/gtest.h>
#include "../includes/Matrix.hpp"
#include "../includes/Tuple.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Transformations.hpp"

// ---------------------------------------------------
// Matrix Transformation

TEST(MatrixTransformations, MultiplyPointByTranslationMatrix)
{
    matrix<4> transform = Translation(5.0f, -3.0f, 2.0f);
    tuple p = createPoint(-3.0f, 4.0f, 5.0f);
    
    EXPECT_EQ(transform * p, createPoint(2.0f, 1.0f, 7.0f));
}

TEST(MatrixTransformations, MultiplyPointByInverseOfTranslationMatrix)
{
    matrix<4> tranform = Translation(5.0f, -3.0f, 2.0f);
    matrix<4> inv = tranform.inverse();

    tuple p = createPoint(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(inv * p, createPoint(-8.0f, 7.0f, 3.0f));
}

TEST(MatrixTransformations, MultiplyTranslationMatrixByVector)
{
    matrix<4> transform = Translation(5.0f, -3.0f, 2.0f);
    tuple p = createVector(-3.0f, 4.0f, 5.0f);

    EXPECT_EQ(transform * p, p);
}