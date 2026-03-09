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
// Suit 2: Basic Tuple Operations

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

// ------------------------------------------------------
// Suit 3: Vector Operations

TEST(vectorOperations, MultiplicationByScalar)
{
	tuple a(1.0, -2.0, 3.0, -4.0);
	tuple result = multiplyTupleByScalar(a, 3.5);

	EXPECT_FLOAT_EQ(result.x, 3.5);
	EXPECT_FLOAT_EQ(result.y, -7.0);
	EXPECT_FLOAT_EQ(result.z, 10.5);
	EXPECT_FLOAT_EQ(result.w, -14.0);
}

TEST(vectorOperations, DivisionByScalar)
{
	tuple a(1.0, -2.0, 3.0, -4.0);
	tuple result = divideTupleByScalar(a, 2.0);

	EXPECT_FLOAT_EQ(result.x, 0.5);
	EXPECT_FLOAT_EQ(result.y, -1.0);
	EXPECT_FLOAT_EQ(result.z, 1.5);
	EXPECT_FLOAT_EQ(result.w, -2.0);
}

TEST(VectorOperations, GetVectorScalar)
{
	tuple a = createVector(1.0, 0.0, 0.0);
	tuple b = createVector(0.0, 1.0, 0.0);
	tuple c = createVector(0.0, 0.0, 1.0);
	tuple d = createVector(1.0, 2.0, 3.0);
	tuple e = createVector(-1.0, -2.0, -3.0);

	EXPECT_FLOAT_EQ(getVectorMagnitude(a), 1.0);
	EXPECT_FLOAT_EQ(getVectorMagnitude(b), 1.0);
	EXPECT_FLOAT_EQ(getVectorMagnitude(c), 1.0);
	EXPECT_FLOAT_EQ(getVectorMagnitude(d), sqrt(14));
	EXPECT_FLOAT_EQ(getVectorMagnitude(e), sqrt(14));
}

TEST(VectorOperations, NormalizeVector)
{
	tuple a = createVector(4.0, 0.0, 0.0);
	tuple a_n = normalizeVector(a);
	tuple b = createVector(1.0, 2.0, 3.0);
	tuple b_n = normalizeVector(b);
	tuple c = createVector(1 / sqrt(14), 1 / sqrt(14), 1 / sqrt(14));
	tuple c_n = normalizeVector(c);

	EXPECT_FLOAT_EQ(getVectorMagnitude(a_n), 1.0);
	EXPECT_FLOAT_EQ(getVectorMagnitude(b_n), 1.0);
	EXPECT_FLOAT_EQ(getVectorMagnitude(c_n), 1.0);
}

TEST(VectorOperations, VectorsDotProduct)
{
	tuple a = createVector(1.0, 2.0, 3.0);
	tuple b = createVector(2.0, 3.0, 4.0);

	EXPECT_FLOAT_EQ(dotProduct(a, b), 20.0);
}

TEST(VectorOperations, VectorsCrossProduct)
{
	tuple a = createVector(1.0, 2.0, 3.0);
	tuple b = createVector(2.0, 3.0, 4.0);

	tuple a_first = crossProduct(a, b);

	EXPECT_FLOAT_EQ(a_first.x, -1.0f);
	EXPECT_FLOAT_EQ(a_first.y, 2.0f);
	EXPECT_FLOAT_EQ(a_first.z, -1.0f);
	EXPECT_FLOAT_EQ(a_first.w, 0.0f);

	tuple b_first = crossProduct(b, a);

	EXPECT_FLOAT_EQ(b_first.x, 1.0f);
	EXPECT_FLOAT_EQ(b_first.y, -2.0f);
	EXPECT_FLOAT_EQ(b_first.z, 1.0f);
	EXPECT_FLOAT_EQ(b_first.w, 0.0f);
}

// ------------------------------------------------------
// Suit 4: Utilities

TEST(Utilities, Equality)
{
	// 1. Exact equality
    EXPECT_TRUE(equal(1.0f, 1.0f));
    EXPECT_TRUE(equal(-5.5f, -5.5f));

    // 2. Near equality (within epsilon)
    EXPECT_TRUE(equal(1.000001f, 1.000002f));
    EXPECT_TRUE(equal(0.0f, 0.0000001f));

    // 3. Boundary case (Exactly EPSILON apart)
    EXPECT_FALSE(equal(1.0f, 1.00001f)); 

    // 4. Far apart
    EXPECT_FALSE(equal(1.0f, 1.1f));
    EXPECT_FALSE(equal(1.0f, -1.0f));
}