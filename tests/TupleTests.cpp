#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

// ---------------------------------------------------
// Tuple Creation

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
// Tuple Comparison

// Test identical tuples
TEST(TupleComparison, IdenticalTuplesAreEqual) {
    tuple a(1.0f, -2.0f, 3.5f, 1.0f);
    tuple b(1.0f, -2.0f, 3.5f, 1.0f);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples within epsilon margin (should be equal)
TEST(TupleComparison, TuplesWithinEpsilonAreEqual) {
    tuple a(1.0f, 1.0f, 1.0f, 1.0f);
    // Difference is 0.000001, which is smaller than 0.00001 EPSILON
    tuple b(1.000001f, 0.999999f, 1.000000f, 1.000000f);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples slightly outside epsilon margin (should not be equal)
TEST(TupleComparison, TuplesOutsideEpsilonAreNotEqual) {
    tuple a(1.0f, 1.0f, 1.0f, 1.0f);
    // Difference is 0.0001, which is larger than 0.00001 EPSILON
    tuple b(1.0001f, 1.0f, 1.0f, 1.0f);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Test completely different tuples
TEST(TupleComparison, DifferentTuplesAreNotEqual) {
    tuple a(1.0f, 2.0f, 3.0f, 1.0f);
    tuple b(4.0f, 5.0f, 6.0f, 0.0f);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// ---------------------------------------------------
// Subscript Operator Tests

TEST(TupleSubscript, AccessingComponentsByIndex)
{
    tuple t(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_FLOAT_EQ(t[0], 1.0f);
    EXPECT_FLOAT_EQ(t[1], 2.0f);
    EXPECT_FLOAT_EQ(t[2], 3.0f);
    EXPECT_FLOAT_EQ(t[3], 4.0f);
}

TEST(TupleSubscript, ModifyingComponentsByIndex)
{
    tuple t;

    t[0] = 10.5f;
    t[1] = 20.5f;
    t[2] = 30.5f;
    t[3] = 1.0f;

	EXPECT_TRUE(t == tuple(10.5f, 20.5f, 30.5f, 1.0f));
}

TEST(TupleSubscript, ConstAccess)
{
    const tuple t(5.0f, 6.0f, 7.0f, 8.0f);
    
    // This calls the const version of operator[]
    float val = t[2];
    
    EXPECT_FLOAT_EQ(val, 7.0f);
}

TEST(TupleSubscript, IndexOutOfBoundsThrowsException)
{
    tuple t(1, 2, 3, 4);

    // Verify that indices outside 0-3 throw std::out_of_range
    EXPECT_THROW(t[4], std::out_of_range);
    EXPECT_THROW(t[100], std::out_of_range);
}

// ------------------------------------------------------
// Basic Tuple Operations

TEST(TupleOperations, AddPointToVector)
{
    tuple p1 = createPoint(3.1f, -2, 5);
    tuple v1 = createVector(-2, 3, 1);

    // Operator Overload
    tuple v2 = p1 + v1;

    EXPECT_TRUE(v2 == tuple(1.1f, 1.0f, 6.0f, 1.0f));
}

TEST(TupleOperations, AddVectorToVector)
{
    tuple v1 = createVector(3, -2, 5.1f);
    tuple v2 = createVector(-2, 3, 1);

    // Operator Overload
    tuple v3 = v1 + v2;

    EXPECT_TRUE(v3 == tuple(1.0f, 1.0f, 6.1f, 0.0f));
}

TEST(TupleOperations, SubtractPointFromPoint)
{
    tuple p1 = createPoint(3, 2, 1);
    tuple p2 = createPoint(5, 6, 7);

    // Operator Overload
    tuple v1 = p1 - p2;

    EXPECT_TRUE(v1 == tuple(-2.0f, -4.0f, -6.0f, 0.0f));
}

TEST(TupleOperations, SubtractVectorFromPoint)
{
    tuple p1 = createPoint(3, 2, 1);
    tuple v1 = createVector(5, 6, 7);

    // Operator Overload
    tuple p2 = p1 - v1;

    EXPECT_TRUE(p2 == tuple(-2.0f, -4.0f, -6.0f, 1.0f));
}

TEST(TupleOperations, SubtractVectorFromVector)
{
    tuple v1 = createVector(3, 2, 1);
    tuple v2 = createVector(5, 6, 7);

    // Operator Overload
    tuple v3 = v1 - v2;

    EXPECT_TRUE(v3 == tuple(-2.0f, -4.0f, -6.0f, 0.0f));
}

TEST(TupleOperations, NegateTuple)
{
    tuple a = tuple(1, -2, 3, -4);
    tuple b = negateTuple(a);

    EXPECT_TRUE(b == tuple(-1.0f, 2.0f, -3.0f, 4.0f));
}

// ------------------------------------------------------
// Vector Operations

TEST(vectorOperations, MultiplicationByScalar)
{
    tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    tuple v2 = v1 * 3.5;

    EXPECT_TRUE(v2 == tuple(3.5f, -7.0f, 10.5f, -14.0f));
}

TEST(vectorOperations, DivisionByScalar)
{
    tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    tuple v2 = v1 / 2;

    EXPECT_TRUE(v2 == tuple(0.5f, -1.0f, 1.5f, -2.0f)); 
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
    EXPECT_FLOAT_EQ(getVectorMagnitude(d), std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(getVectorMagnitude(e), std::sqrt(14.0f));
}

TEST(VectorOperations, NormalizeVector)
{
    tuple a = createVector(4.0, 0.0, 0.0);
    tuple a_n = normalizeVector(a);
    tuple b = createVector(1.0, 2.0, 3.0);
    tuple b_n = normalizeVector(b);
    tuple c = createVector(1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f));
    tuple c_n = normalizeVector(c);

    EXPECT_TRUE(a_n == tuple(1.0f, 0.0f, 0.0f, 0.0f));
    EXPECT_TRUE(b_n == tuple(1.0f / std::sqrt(14.0f), 2.0f / std::sqrt(14.0f), 3.0f / std::sqrt(14.0f), 0.0f));
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

    EXPECT_TRUE(a_first == tuple(-1.0f, 2.0f, -1.0f, 0.0f));

    tuple b_first = crossProduct(b, a);

    EXPECT_TRUE(b_first == tuple(1.0f, -2.0f, 1.0f, 0.0f));
}

// ------------------------------------------------------
// Utilities

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