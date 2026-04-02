#include <gtest/gtest.h>
#include "../includes/Tuple.hpp"
#include "../includes/Colors.hpp"

// ---------------------------------------------------
// Suit 1: Tuple Creation

TEST(TupleCreation, CreatePointSetsWToOne)
{
	tuple p1 = createPoint(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(p1.x, 4.3f);
	EXPECT_FLOAT_EQ(p1.y, -4.2f);
	EXPECT_FLOAT_EQ(p1.z, 3.1f);
	EXPECT_FLOAT_EQ(p1.w, 1.0f);
}

TEST(TupleCreation, CreateVectorSetsWToZero)
{
	tuple v1 = createVector(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(v1.x, 4.3f);
	EXPECT_FLOAT_EQ(v1.y, -4.2f);
	EXPECT_FLOAT_EQ(v1.z, 3.1f);
	EXPECT_FLOAT_EQ(v1.w, 0.0f);
}

// ------------------------------------------------------
// Suit 2: Basic Tuple Operations

TEST(TupleOperations, AddPointToVector)
{
	tuple p1 = createPoint(3.1f, -2, 5);
	tuple v1 = createVector(-2, 3, 1);

	// Operator Overload
	tuple v2 = p1 + v1;

	EXPECT_FLOAT_EQ(v2.x, 1.1f);
	EXPECT_FLOAT_EQ(v2.y, 1.0f);
	EXPECT_FLOAT_EQ(v2.z, 6.0f);
	EXPECT_FLOAT_EQ(v2.w, 1.0f);
}

TEST(TupleOperations, AddVectorToVector)
{
	tuple v1 = createVector(3, -2, 5.1f);
	tuple v2 = createVector(-2, 3, 1);

	// Operator Overload
	tuple v3 = v1 + v2;

	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 1.0f);
	EXPECT_FLOAT_EQ(v3.z, 6.1f);
	EXPECT_FLOAT_EQ(v3.w, 0.0f);
}

TEST(TupleOperations, SubtractPointFromPoint)
{
	tuple p1 = createPoint(3, 2, 1);
	tuple p2 = createPoint(5, 6, 7);

	// Operator Overload
	tuple v1 = p1 - p2;

	EXPECT_FLOAT_EQ(v1.x, -2.0f);
	EXPECT_FLOAT_EQ(v1.y, -4.0f);
	EXPECT_FLOAT_EQ(v1.z, -6.0f);
	EXPECT_FLOAT_EQ(v1.w, 0.0f);
}

TEST(TupleOperations, SubtractVectorFromPoint)
{
	tuple p1 = createPoint(3, 2, 1);
	tuple v1 = createVector(5, 6, 7);

	// Operator Overload
	tuple p2 = p1 - v1;

	EXPECT_FLOAT_EQ(p2.x, -2.0f);
	EXPECT_FLOAT_EQ(p2.y, -4.0f);
	EXPECT_FLOAT_EQ(p2.z, -6.0f);
	EXPECT_FLOAT_EQ(p2.w, 1.0f);
}

TEST(TupleOperations, SubtractVectorFromVector)
{
	tuple v1 = createVector(3, 2, 1);
	tuple v2 = createVector(5, 6, 7);

	// Operator Overload
	tuple v3 = v1 - v2;

	EXPECT_FLOAT_EQ(v3.x, -2.0f);
	EXPECT_FLOAT_EQ(v3.y, -4.0f);
	EXPECT_FLOAT_EQ(v3.z, -6.0f);
	EXPECT_FLOAT_EQ(v3.w, 0.0f);
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
	tuple v1(1.0, -2.0, 3.0, -4.0);

	// Operator Overload
	tuple v2 = v1 * 3.5;

	EXPECT_FLOAT_EQ(v2.x, 3.5);
	EXPECT_FLOAT_EQ(v2.y, -7.0);
	EXPECT_FLOAT_EQ(v2.z, 10.5);
	EXPECT_FLOAT_EQ(v2.w, -14.0);
}

TEST(vectorOperations, DivisionByScalar)
{
	tuple v1(1.0, -2.0, 3.0, -4.0);

	// Operator Overload
	tuple v2 = v1 / 2;

	EXPECT_FLOAT_EQ(v2.x, 0.5);
	EXPECT_FLOAT_EQ(v2.y, -1.0);
	EXPECT_FLOAT_EQ(v2.z, 1.5);
	EXPECT_FLOAT_EQ(v2.w, -2.0); 
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

// ------------------------------------------------------
// Suit 5: Color Creation

TEST(ColorCreation, CreateColorTuple)
{
	color c = createColor(-0.5, 0.4, 1.7);

	EXPECT_FLOAT_EQ(c.r, -0.5);
	EXPECT_FLOAT_EQ(c.g, 0.4);
	EXPECT_FLOAT_EQ(c.b, 1.7);
}

// ------------------------------------------------------
// Suit 5: Color Creation

TEST(ColorOperation, AddTwoColors)
{
	color c1 = createColor(0.9, 0.6, 0.75);
	color c2 = createColor(0.7, 0.1, 0.25);

	// Operator Overload
	color c_result = c1 + c2;

	EXPECT_FLOAT_EQ(c_result.r, 1.6);
	EXPECT_FLOAT_EQ(c_result.g, 0.7);
	EXPECT_FLOAT_EQ(c_result.b, 1.0);
}

TEST(ColorOperation, SubtractTwoColors)
{
	color c1 = createColor(0.9, 0.6, 0.75);
	color c2 = createColor(0.7, 0.1, 0.25);

	// Operator Overload
	color c_result = c1 - c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.2);
	EXPECT_FLOAT_EQ(c_result.g, 0.5);
	EXPECT_FLOAT_EQ(c_result.b, 0.5);
}

TEST(ColorOperation, MultiplicationByScalar)
{
	color c1 = createColor(0.2, 0.3, 0.4);

	// Operator Overload
	color c_result = c1 * 2;

	EXPECT_FLOAT_EQ(c_result.r, 0.4);
	EXPECT_FLOAT_EQ(c_result.g, 0.6);
	EXPECT_FLOAT_EQ(c_result.b, 0.8);
}

TEST(ColorOperation, MultiplyTwoColors)
{
	color c1 = createColor(1, 0.2, 0.4);
	color c2 = createColor(0.9, 1, 0.1);

	// Operator Overload
	color c_result = c1 * c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.9);
	EXPECT_FLOAT_EQ(c_result.g, 0.2);
	EXPECT_FLOAT_EQ(c_result.b, 0.04);
}