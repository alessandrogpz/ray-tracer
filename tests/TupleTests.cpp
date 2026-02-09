#include <gtest/gtest.h>
#include "../src/Tuple.h"

// Test 1: createPoint() should set w to 1.0
TEST(TupleCreation, CreatePointSetsWToOne)
{
    tuple p = createPoint(4.3f, -4.2f, 3.1f);

    EXPECT_FLOAT_EQ(p.x, 4.3f);
    EXPECT_FLOAT_EQ(p.y, -4.2f);
    EXPECT_FLOAT_EQ(p.z, 3.1f);
    EXPECT_FLOAT_EQ(p.w, 1.0f); // This is the crucial part for points
}

// Test 2: createVector() should set w to 0.0
TEST(TupleCreation, CreateVectorSetsWToZero)
{
    tuple v = createVector(4.3f, -4.2f, 3.1f);

    EXPECT_FLOAT_EQ(v.x, 4.3f);
    EXPECT_FLOAT_EQ(v.y, -4.2f);
    EXPECT_FLOAT_EQ(v.z, 3.1f);
    EXPECT_FLOAT_EQ(v.w, 0.0f); // This is the crucial part for vectors
}

// Test 3: Precision check (Mental Prep for future chapters)
TEST(TupleCreation, FloatingPointPrecision)
{
    // In ray tracing, 1.0/3.0 might be slightly off.
    // EXPECT_NEAR allows for a tiny margin of error (epsilon).
    tuple p = createPoint(1.0f / 3.0f, 0.0f, 0.0f);
    EXPECT_NEAR(p.x, 0.333333f, 0.00001f);
}