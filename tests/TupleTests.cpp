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

TEST(TupleCreation, CreateGenericTuple)
{
    tuple t1 = tuple(4.3f, -4.2f, 3.1f, -5.7f);

    EXPECT_FLOAT_EQ(t1.x, 4.3f);
    EXPECT_FLOAT_EQ(t1.y, -4.2f);
    EXPECT_FLOAT_EQ(t1.z, 3.1f);
    EXPECT_FLOAT_EQ(t1.w, -5.7f);
}

TEST(TupleCreation, CreatePointSetsWToOne)
{
	point p1 = createPoint(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(p1.x, 4.3f);
	EXPECT_FLOAT_EQ(p1.y, -4.2f);
	EXPECT_FLOAT_EQ(p1.z, 3.1f);
	EXPECT_FLOAT_EQ(p1.w, 1.0f);
}

TEST(TupleCreation, CreateVectorSetsWToZero)
{
	vector v1 = createVector(4.3f, -4.2f, 3.1f);

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

// Test completely different types
TEST(TupleComparison, DifferentTupleTypesAreNotEqual)
{
    point a(1.0f, 2.0f, 3.0f);
    vector b(1.0f, 2.0f, 3.0f);

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
    point p1 = createPoint(3.1f, -2, 5);
    vector v1 = createVector(-2, 3, 1);

    // Operator Overload
    point v2 = p1 + v1;

    EXPECT_TRUE(v2 == point(1.1f, 1.0f, 6.0f));
}

TEST(TupleOperations, AddVectorToVector)
{
    vector v1 = createVector(3, -2, 5.1f);
    vector v2 = createVector(-2, 3, 1);

    // Operator Overload
    tuple v3 = v1 + v2;

    EXPECT_TRUE(v3 == vector(1.0f, 1.0f, 6.1f));
}

TEST(TupleOperations, SubtractPointFromPoint)
{
    point p1 = createPoint(3, 2, 1);
    point p2 = createPoint(5, 6, 7);

    // Operator Overload
    vector v1 = p1 - p2;

    EXPECT_TRUE(v1 == vector(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, SubtractVectorFromPoint)
{
    point p1 = createPoint(3, 2, 1);
    vector v1 = createVector(5, 6, 7);

    // Operator Overload
    point p2 = p1 - v1;

    EXPECT_TRUE(p2 == point(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, SubtractVectorFromVector)
{
    vector v1 = createVector(3, 2, 1);
    vector v2 = createVector(5, 6, 7);

    // Operator Overload
    tuple v3 = v1 - v2;

    EXPECT_TRUE(v3 == vector(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, MultiplyGenericTupleByScalar)
{
    tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    tuple v2 = v1 * 3.5;

    EXPECT_TRUE(v2 == tuple(3.5f, -7.0f, 10.5f, -14.0f));
}

TEST(TupleOperations, DivideGenericTupleByScalar)
{
    tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    tuple v2 = v1 / 2;

    EXPECT_TRUE(v2 == tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

// ------------------------------------------------------
// Vector Operations

TEST(VectorOperations, NegateVector)
{
    vector a = vector(1, -2, 3);
    vector b = -a;
    vector c = (negateVector(a));

    EXPECT_TRUE(b == vector(-1.0f, 2.0f, -3.0f));
    EXPECT_TRUE(c == vector(-1.0f, 2.0f, -3.0f));
}

TEST(VectorOperations, MultiplicationByScalar)
{
    vector v1 = createVector(1.0f, -2.0f, 3.0f);

    // Vector Specific Operator Overload
    vector v2 = v1 * 3.5f;

    EXPECT_TRUE(v2 == vector(3.5f, -7.0f, 10.5f));
}

TEST(VectorOperations, DivisionByScalar)
{
    vector v1 = createVector(1.0f, -2.0f, 3.0f);

    // Vector Specific Operator Overload
    vector v2 = v1 / 2.0f;

    EXPECT_TRUE(v2 == vector(0.5f, -1.0f, 1.5f));
}

TEST(VectorOperations, GetVectorScalar)
{
    vector a = createVector(1.0, 0.0, 0.0);
    vector b = createVector(0.0, 1.0, 0.0);
    vector c = createVector(0.0, 0.0, 1.0);
    vector d = createVector(1.0, 2.0, 3.0);
    vector e = createVector(-1.0, -2.0, -3.0);

    EXPECT_FLOAT_EQ(getVectorMagnitude(a), 1.0);
    EXPECT_FLOAT_EQ(getVectorMagnitude(b), 1.0);
    EXPECT_FLOAT_EQ(getVectorMagnitude(c), 1.0);
    EXPECT_FLOAT_EQ(getVectorMagnitude(d), std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(getVectorMagnitude(e), std::sqrt(14.0f));
}

TEST(VectorOperations, NormalizeVector)
{
    vector a = createVector(4.0, 0.0, 0.0);
    vector a_n = normalizeVector(a);
    vector b = createVector(1.0, 2.0, 3.0);
    vector b_n = normalizeVector(b);
    vector c = createVector(1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f));
    vector c_n = normalizeVector(c);

    EXPECT_TRUE(a_n == vector(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(b_n == vector(1.0f / std::sqrt(14.0f), 2.0f / std::sqrt(14.0f), 3.0f / std::sqrt(14.0f)));
    EXPECT_FLOAT_EQ(getVectorMagnitude(c_n), 1.0);
}

TEST(VectorOperations, VectorsDotProduct)
{
    vector a = createVector(1.0, 2.0, 3.0);
    vector b = createVector(2.0, 3.0, 4.0);

    EXPECT_FLOAT_EQ(dotProduct(a, b), 20.0);
}

TEST(VectorOperations, VectorsCrossProduct)
{
    vector a = createVector(1.0, 2.0, 3.0);
    vector b = createVector(2.0, 3.0, 4.0);

    vector a_first = crossProduct(a, b);

    EXPECT_TRUE(a_first == vector(-1.0f, 2.0f, -1.0f));

    vector b_first = crossProduct(b, a);

    EXPECT_TRUE(b_first == vector(1.0f, -2.0f, 1.0f));
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