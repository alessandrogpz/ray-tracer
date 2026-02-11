#include <gtest/gtest.h>
#include "../src/Tuple.h"

TEST(TupleCreation, CreatePointSetsWToOne)
{
    tuple p = createPoint(4.3f, -4.2f, 3.1f);

    EXPECT_FLOAT_EQ(p.x, 4.3f);
    EXPECT_FLOAT_EQ(p.y, -4.2f);
    EXPECT_FLOAT_EQ(p.z, 3.1f);
    EXPECT_FLOAT_EQ(p.w, 1.0f);
}

TEST(TupleCreation, CreateVectorSetsWToZero)
{
    tuple v = createVector(4.3f, -4.2f, 3.1f);

    EXPECT_FLOAT_EQ(v.x, 4.3f);
    EXPECT_FLOAT_EQ(v.y, -4.2f);
    EXPECT_FLOAT_EQ(v.z, 3.1f);
    EXPECT_FLOAT_EQ(v.w, 0.0f);
}

TEST(TupleCreation, FloatingPointPrecision)
{
    // In ray tracing, 1.0/3.0 might be slightly off.
    // EXPECT_NEAR allows for a tiny margin of error (epsilon).
    tuple p = createPoint(1.0f / 3.0f, 0.0f, 0.0f);
    EXPECT_NEAR(p.x, 0.333333f, 0.00001f);
}

// Test 4: addTuple()
TEST(TupleOperations, AddPointToVector)
{
    tuple a = tuple(3.1f, -2, 5, 1);
    tuple b = tuple(-2, 3, 1, 0);
    tuple c = addTuples(a, b);

    EXPECT_FLOAT_EQ(c.x, 1.1f);
    EXPECT_FLOAT_EQ(c.y, 1.0f);
    EXPECT_FLOAT_EQ(c.z, 6.0f);
    EXPECT_FLOAT_EQ(c.w, 1.0f);
}

TEST(TupleOperations, AddVectorToVector)
{
    tuple a = tuple(3, -2, 5.1f, 0);
    tuple b = tuple(-2, 3, 1, 0);
    tuple c = addTuples(a, b);

    EXPECT_FLOAT_EQ(c.x, 1.0f);
    EXPECT_FLOAT_EQ(c.y, 1.0f);
    EXPECT_FLOAT_EQ(c.z, 6.1f);
    EXPECT_FLOAT_EQ(c.w, 0.0f);
}