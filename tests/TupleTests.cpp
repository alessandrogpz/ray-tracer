#include <gtest/gtest.h>
#include "../src/Tuple.hpp"

// ---------------------------------------------------
// Suit 1: Tuple Creation

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

// ------------------------------------------------------
// Suit 2: Tuple Operations

TEST(TupleOperations, AddPointToVector)
{
	tuple a = createPoint(3.1f, -2, 5);
	tuple b = createVector(-2, 3, 1);
	tuple c = addTuples(a, b);

	EXPECT_FLOAT_EQ(c.x, 1.1f);
	EXPECT_FLOAT_EQ(c.y, 1.0f);
	EXPECT_FLOAT_EQ(c.z, 6.0f);
	EXPECT_FLOAT_EQ(c.w, 1.0f);
}

TEST(TupleOperations, AddVectorToVector)
{
	tuple a = createVector(3, -2, 5.1f);
	tuple b = createVector(-2, 3, 1);
	tuple c = addTuples(a, b);

	EXPECT_FLOAT_EQ(c.x, 1.0f);
	EXPECT_FLOAT_EQ(c.y, 1.0f);
	EXPECT_FLOAT_EQ(c.z, 6.1f);
	EXPECT_FLOAT_EQ(c.w, 0.0f);
}

// Test 5: subtractTuples()
TEST(TupleOperations, SubtractPointToPoint)
{
	tuple a = createPoint(3, 2, 1);
	tuple b = createPoint(5, 6, 7);
	tuple c = subtractTuples(a, b);

	EXPECT_FLOAT_EQ(c.x, -2.0f);
	EXPECT_FLOAT_EQ(c.y, -4.0f);
	EXPECT_FLOAT_EQ(c.z, -6.0f);
	EXPECT_FLOAT_EQ(c.w, 0.0f);
}

TEST(TupleOperations, SubtractVectorToPoint)
{
	tuple a = createPoint(3, 2, 1);
	tuple b = createVector(5, 6, 7);
	tuple c = subtractTuples(a, b);

	EXPECT_FLOAT_EQ(c.x, -2.0f);
	EXPECT_FLOAT_EQ(c.y, -4.0f);
	EXPECT_FLOAT_EQ(c.z, -6.0f);
	EXPECT_FLOAT_EQ(c.w, 1.0f);
}

TEST(TupleOperations, SubtractVectorToVector)
{
	tuple a = createVector(3, 2, 1);
	tuple b = createVector(5, 6, 7);
	tuple c = subtractTuples(a, b);

	EXPECT_FLOAT_EQ(c.x, -2.0f);
	EXPECT_FLOAT_EQ(c.y, -4.0f);
	EXPECT_FLOAT_EQ(c.z, -6.0f);
	EXPECT_FLOAT_EQ(c.w, 0.0f);
}

TEST(TupleOperations, NegateTuple)
{
	tuple a = tuple(1, -2, 3, -4);
	tuple b = negateTuple(a);

	EXPECT_FLOAT_EQ(b.x, -1.0f);
	EXPECT_FLOAT_EQ(b.y, 2.0f);
	EXPECT_FLOAT_EQ(b.z, -3.0f);
	EXPECT_FLOAT_EQ(b.w, 4.0f);
}
