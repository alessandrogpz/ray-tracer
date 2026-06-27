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
// Tuple Creation

TEST(TupleCreation, CreateGenericTuple)
{
    Tuple t1 = Tuple(4.3f, -4.2f, 3.1f, -5.7f);

    EXPECT_FLOAT_EQ(t1.x, 4.3f);
    EXPECT_FLOAT_EQ(t1.y, -4.2f);
    EXPECT_FLOAT_EQ(t1.z, 3.1f);
    EXPECT_FLOAT_EQ(t1.w, -5.7f);
}

TEST(TupleCreation, CreatePointSetsWToOne)
{
	Point p1 = createPoint(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(p1.x, 4.3f);
	EXPECT_FLOAT_EQ(p1.y, -4.2f);
	EXPECT_FLOAT_EQ(p1.z, 3.1f);
	EXPECT_FLOAT_EQ(p1.w, 1.0f);
}

TEST(TupleCreation, CreateVectorSetsWToZero)
{
	Vector v1 = createVector(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(v1.x, 4.3f);
	EXPECT_FLOAT_EQ(v1.y, -4.2f);
	EXPECT_FLOAT_EQ(v1.z, 3.1f);
	EXPECT_FLOAT_EQ(v1.w, 0.0f);
}

// ------------------------------------------------------
// Tuple Comparison

// Test identical tuples
TEST(TupleComparison, IdenticalTuplesAreEqual) {
    Tuple a(1.0f, -2.0f, 3.5f, 1.0f);
    Tuple b(1.0f, -2.0f, 3.5f, 1.0f);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples within epsilon margin (should be equal)
TEST(TupleComparison, TuplesWithinEpsilonAreEqual) {
    Tuple a(1.0f, 1.0f, 1.0f, 1.0f);
    // Difference is 0.000001f, which is smaller than 0.00001f EPSILON
    Tuple b(1.000001f, 0.999999f, 1.000000f, 1.000000f);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples slightly outside epsilon margin (should not be equal)
TEST(TupleComparison, TuplesOutsideEpsilonAreNotEqual) {
    Tuple a(1.0f, 1.0f, 1.0f, 1.0f);
    // Difference is 0.0001f, which is larger than 0.00001f EPSILON
    Tuple b(1.0001f, 1.0f, 1.0f, 1.0f);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Test completely different tuples
TEST(TupleComparison, DifferentTuplesAreNotEqual) {
    Tuple a(1.0f, 2.0f, 3.0f, 1.0f);
    Tuple b(4.0f, 5.0f, 6.0f, 0.0f);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Test completely different types
TEST(TupleComparison, DifferentTupleTypesAreNotEqual)
{
    Point a(1.0f, 2.0f, 3.0f);
    Vector b(1.0f, 2.0f, 3.0f);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// ---------------------------------------------------
// Subscript Operator Tests

TEST(TupleSubscript, AccessingComponentsByIndex)
{
    Tuple t(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_FLOAT_EQ(t[0], 1.0f);
    EXPECT_FLOAT_EQ(t[1], 2.0f);
    EXPECT_FLOAT_EQ(t[2], 3.0f);
    EXPECT_FLOAT_EQ(t[3], 4.0f);
}

TEST(TupleSubscript, ModifyingComponentsByIndex)
{
    Tuple t;

    t[0] = 10.5f;
    t[1] = 20.5f;
    t[2] = 30.5f;
    t[3] = 1.0f;

	EXPECT_TRUE(t == Tuple(10.5f, 20.5f, 30.5f, 1.0f));
}

TEST(TupleSubscript, ConstAccess)
{
    const Tuple t(5.0f, 6.0f, 7.0f, 8.0f);
    
    // This calls the const version of operator[]
    float val = t[2];
    
    EXPECT_FLOAT_EQ(val, 7.0f);
}

TEST(TupleSubscript, IndexOutOfBoundsThrowsException)
{
    Tuple t(1, 2, 3, 4);

    // Verify that indices outside 0-3 throw std::out_of_range
    EXPECT_THROW(t[4], std::out_of_range);
    EXPECT_THROW(t[100], std::out_of_range);
}

// ------------------------------------------------------
// Basic Tuple Operations

TEST(TupleOperations, AddPointToVector)
{
    Point p1 = createPoint(3.1f, -2, 5);
    Vector v1 = createVector(-2, 3, 1);

    // Operator Overload
    Point v2 = p1 + v1;

    EXPECT_TRUE(v2 == Point(1.1f, 1.0f, 6.0f));
}

TEST(TupleOperations, AddVectorToVector)
{
    Vector v1 = createVector(3, -2, 5.1f);
    Vector v2 = createVector(-2, 3, 1);

    // Operator Overload
    Tuple v3 = v1 + v2;

    EXPECT_TRUE(v3 == Vector(1.0f, 1.0f, 6.1f));
}

TEST(TupleOperations, SubtractPointFromPoint)
{
    Point p1 = createPoint(3, 2, 1);
    Point p2 = createPoint(5, 6, 7);

    // Operator Overload
    Vector v1 = p1 - p2;

    EXPECT_TRUE(v1 == Vector(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, SubtractVectorFromPoint)
{
    Point p1 = createPoint(3, 2, 1);
    Vector v1 = createVector(5, 6, 7);

    // Operator Overload
    Point p2 = p1 - v1;

    EXPECT_TRUE(p2 == Point(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, SubtractVectorFromVector)
{
    Vector v1 = createVector(3, 2, 1);
    Vector v2 = createVector(5, 6, 7);

    // Operator Overload
    Tuple v3 = v1 - v2;

    EXPECT_TRUE(v3 == Vector(-2.0f, -4.0f, -6.0f));
}

TEST(TupleOperations, MultiplyGenericTupleByScalar)
{
    Tuple v1(1.0f, -2.0f, 3.0f, -4.0f);

    // Operator Overload
    Tuple v2 = v1 * 3.5f;

    EXPECT_TRUE(v2 == Tuple(3.5f, -7.0f, 10.5f, -14.0f));
}

TEST(TupleOperations, DivideGenericTupleByScalar)
{
    Tuple v1(1.0f, -2.0f, 3.0f, -4.0f);

    // Operator Overload
    Tuple v2 = v1 / 2;

    EXPECT_TRUE(v2 == Tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

// ------------------------------------------------------
// Vector Operations

TEST(VectorOperations, NegateVector)
{
    Vector a = Vector(1, -2, 3);
    Vector b = -a;
    Vector c = (negateVector(a));

    EXPECT_TRUE(b == Vector(-1.0f, 2.0f, -3.0f));
    EXPECT_TRUE(c == Vector(-1.0f, 2.0f, -3.0f));
}

TEST(VectorOperations, MultiplicationByScalar)
{
    Vector v1 = createVector(1.0f, -2.0f, 3.0f);

    // Vector Specific Operator Overload
    Vector v2 = v1 * 3.5f;

    EXPECT_TRUE(v2 == Vector(3.5f, -7.0f, 10.5f));
}

TEST(VectorOperations, DivisionByScalar)
{
    Vector v1 = createVector(1.0f, -2.0f, 3.0f);

    // Vector Specific Operator Overload
    Vector v2 = v1 / 2.0f;

    EXPECT_TRUE(v2 == Vector(0.5f, -1.0f, 1.5f));
}

TEST(VectorOperations, GetVectorScalar)
{
    Vector a = createVector(1.0f, 0.0f, 0.0f);
    Vector b = createVector(0.0f, 1.0f, 0.0f);
    Vector c = createVector(0.0f, 0.0f, 1.0f);
    Vector d = createVector(1.0f, 2.0f, 3.0f);
    Vector e = createVector(-1.0f, -2.0f, -3.0f);

    EXPECT_FLOAT_EQ(getVectorMagnitude(a), 1.0f);
    EXPECT_FLOAT_EQ(getVectorMagnitude(b), 1.0f);
    EXPECT_FLOAT_EQ(getVectorMagnitude(c), 1.0f);
    EXPECT_FLOAT_EQ(getVectorMagnitude(d), std::sqrt(14.0f));
    EXPECT_FLOAT_EQ(getVectorMagnitude(e), std::sqrt(14.0f));
}

TEST(VectorOperations, NormalizeVector)
{
    Vector a = createVector(4.0f, 0.0f, 0.0f);
    Vector a_n = normalizeVector(a);
    Vector b = createVector(1.0f, 2.0f, 3.0f);
    Vector b_n = normalizeVector(b);
    Vector c = createVector(1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f), 1 / std::sqrt(14.0f));
    Vector c_n = normalizeVector(c);

    EXPECT_TRUE(a_n == Vector(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(b_n == Vector(1.0f / std::sqrt(14.0f), 2.0f / std::sqrt(14.0f), 3.0f / std::sqrt(14.0f)));
    EXPECT_FLOAT_EQ(getVectorMagnitude(c_n), 1.0f);
}

TEST(VectorOperations, VectorsDotProduct)
{
    Vector a = createVector(1.0f, 2.0f, 3.0f);
    Vector b = createVector(2.0f, 3.0f, 4.0f);

    EXPECT_FLOAT_EQ(dotProduct(a, b), 20.0f);
}

TEST(VectorOperations, VectorsCrossProduct)
{
    Vector a = createVector(1.0f, 2.0f, 3.0f);
    Vector b = createVector(2.0f, 3.0f, 4.0f);

    Vector a_first = crossProduct(a, b);

    EXPECT_TRUE(a_first == Vector(-1.0f, 2.0f, -1.0f));

    Vector b_first = crossProduct(b, a);

    EXPECT_TRUE(b_first == Vector(1.0f, -2.0f, 1.0f));
}

// ------------------------------------------------------
TEST(VectorReflection, ReflectVectorApproaching45)
{
    Vector a = createVector(1, -1, 0);
    Vector n = createVector(0, 1, 0);
    Vector reflection = reflect(a, n);

    EXPECT_EQ(reflection, createVector(1, 1, 0));
}

TEST(VectorReflection, ReflectAVectorOffSlantedSurface)
{
    Vector a = createVector(0, -1, 0);
    Vector n = createVector(std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0);
    Vector reflection = reflect(a, n);

    EXPECT_EQ(reflection, createVector(1, 0, 0));
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
    EXPECT_FALSE(equal(1.0f, 1.0001f)); 

    // 4. Far apart
    EXPECT_FALSE(equal(1.0f, 1.1f));
    EXPECT_FALSE(equal(1.0f, -1.0f));
}