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
    Tuple t1 = Tuple(4.3, -4.2, 3.1, -5.7);

    EXPECT_DOUBLE_EQ(t1.x, 4.3);
    EXPECT_DOUBLE_EQ(t1.y, -4.2);
    EXPECT_DOUBLE_EQ(t1.z, 3.1);
    EXPECT_DOUBLE_EQ(t1.w, -5.7);
}

TEST(TupleCreation, CreatePointSetsWToOne)
{
	Point p1 = createPoint(4.3, -4.2, 3.1);

	EXPECT_DOUBLE_EQ(p1.x, 4.3);
	EXPECT_DOUBLE_EQ(p1.y, -4.2);
	EXPECT_DOUBLE_EQ(p1.z, 3.1);
	EXPECT_DOUBLE_EQ(p1.w, 1.0);
}

TEST(TupleCreation, CreateVectorSetsWToZero)
{
	Vector v1 = createVector(4.3, -4.2, 3.1);

	EXPECT_DOUBLE_EQ(v1.x, 4.3);
	EXPECT_DOUBLE_EQ(v1.y, -4.2);
	EXPECT_DOUBLE_EQ(v1.z, 3.1);
	EXPECT_DOUBLE_EQ(v1.w, 0.0);
}

// ------------------------------------------------------
// Tuple Comparison

// Test identical tuples
TEST(TupleComparison, IdenticalTuplesAreEqual) {
    Tuple a(1.0, -2.0, 3.5, 1.0);
    Tuple b(1.0, -2.0, 3.5, 1.0);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples within epsilon margin (should be equal)
TEST(TupleComparison, TuplesWithinEpsilonAreEqual) {
    Tuple a(1.0, 1.0, 1.0, 1.0);
    // Difference is 0.000001, which is smaller than 0.00001 EPSILON
    Tuple b(1.000001, 0.999999, 1.000000, 1.000000);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

// Test tuples slightly outside epsilon margin (should not be equal)
TEST(TupleComparison, TuplesOutsideEpsilonAreNotEqual) {
    Tuple a(1.0, 1.0, 1.0, 1.0);
    // Difference is 0.0001, which is larger than 0.00001 EPSILON
    Tuple b(1.0001, 1.0, 1.0, 1.0);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Test completely different tuples
TEST(TupleComparison, DifferentTuplesAreNotEqual) {
    Tuple a(1.0, 2.0, 3.0, 1.0);
    Tuple b(4.0, 5.0, 6.0, 0.0);
    
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// Test completely different types
TEST(TupleComparison, DifferentTupleTypesAreNotEqual)
{
    Point a(1.0, 2.0, 3.0);
    Vector b(1.0, 2.0, 3.0);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

// ---------------------------------------------------
// Subscript Operator Tests

TEST(TupleSubscript, AccessingComponentsByIndex)
{
    Tuple t(1.0, 2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(t[0], 1.0);
    EXPECT_DOUBLE_EQ(t[1], 2.0);
    EXPECT_DOUBLE_EQ(t[2], 3.0);
    EXPECT_DOUBLE_EQ(t[3], 4.0);
}

TEST(TupleSubscript, ModifyingComponentsByIndex)
{
    Tuple t;

    t[0] = 10.5;
    t[1] = 20.5;
    t[2] = 30.5;
    t[3] = 1.0;

	EXPECT_TRUE(t == Tuple(10.5, 20.5, 30.5, 1.0));
}

TEST(TupleSubscript, ConstAccess)
{
    const Tuple t(5.0, 6.0, 7.0, 8.0);
    
    // This calls the const version of operator[]
    double val = t[2];
    
    EXPECT_DOUBLE_EQ(val, 7.0);
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
    Point p1 = createPoint(3.1, -2, 5);
    Vector v1 = createVector(-2, 3, 1);

    // Operator Overload
    Point v2 = p1 + v1;

    EXPECT_TRUE(v2 == Point(1.1, 1.0, 6.0));
}

TEST(TupleOperations, AddVectorToVector)
{
    Vector v1 = createVector(3, -2, 5.1);
    Vector v2 = createVector(-2, 3, 1);

    // Operator Overload
    Tuple v3 = v1 + v2;

    EXPECT_TRUE(v3 == Vector(1.0, 1.0, 6.1));
}

TEST(TupleOperations, SubtractPointFromPoint)
{
    Point p1 = createPoint(3, 2, 1);
    Point p2 = createPoint(5, 6, 7);

    // Operator Overload
    Vector v1 = p1 - p2;

    EXPECT_TRUE(v1 == Vector(-2.0, -4.0, -6.0));
}

TEST(TupleOperations, SubtractVectorFromPoint)
{
    Point p1 = createPoint(3, 2, 1);
    Vector v1 = createVector(5, 6, 7);

    // Operator Overload
    Point p2 = p1 - v1;

    EXPECT_TRUE(p2 == Point(-2.0, -4.0, -6.0));
}

TEST(TupleOperations, SubtractVectorFromVector)
{
    Vector v1 = createVector(3, 2, 1);
    Vector v2 = createVector(5, 6, 7);

    // Operator Overload
    Tuple v3 = v1 - v2;

    EXPECT_TRUE(v3 == Vector(-2.0, -4.0, -6.0));
}

TEST(TupleOperations, MultiplyGenericTupleByScalar)
{
    Tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    Tuple v2 = v1 * 3.5;

    EXPECT_TRUE(v2 == Tuple(3.5, -7.0, 10.5, -14.0));
}

TEST(TupleOperations, DivideGenericTupleByScalar)
{
    Tuple v1(1.0, -2.0, 3.0, -4.0);

    // Operator Overload
    Tuple v2 = v1 / 2;

    EXPECT_TRUE(v2 == Tuple(0.5, -1.0, 1.5, -2.0));
}

// ------------------------------------------------------
// Vector Operations

TEST(VectorOperations, NegateVector)
{
    Vector a = Vector(1, -2, 3);
    Vector b = -a;
    Vector c = (negateVector(a));

    EXPECT_TRUE(b == Vector(-1.0, 2.0, -3.0));
    EXPECT_TRUE(c == Vector(-1.0, 2.0, -3.0));
}

TEST(VectorOperations, MultiplicationByScalar)
{
    Vector v1 = createVector(1.0, -2.0, 3.0);

    // Vector Specific Operator Overload
    Vector v2 = v1 * 3.5;

    EXPECT_TRUE(v2 == Vector(3.5, -7.0, 10.5));
}

TEST(VectorOperations, DivisionByScalar)
{
    Vector v1 = createVector(1.0, -2.0, 3.0);

    // Vector Specific Operator Overload
    Vector v2 = v1 / 2.0;

    EXPECT_TRUE(v2 == Vector(0.5, -1.0, 1.5));
}

TEST(VectorOperations, GetVectorScalar)
{
    Vector a = createVector(1.0, 0.0, 0.0);
    Vector b = createVector(0.0, 1.0, 0.0);
    Vector c = createVector(0.0, 0.0, 1.0);
    Vector d = createVector(1.0, 2.0, 3.0);
    Vector e = createVector(-1.0, -2.0, -3.0);

    EXPECT_DOUBLE_EQ(getVectorMagnitude(a), 1.0);
    EXPECT_DOUBLE_EQ(getVectorMagnitude(b), 1.0);
    EXPECT_DOUBLE_EQ(getVectorMagnitude(c), 1.0);
    EXPECT_DOUBLE_EQ(getVectorMagnitude(d), std::sqrt(14.0));
    EXPECT_DOUBLE_EQ(getVectorMagnitude(e), std::sqrt(14.0));
}

TEST(VectorOperations, NormalizeVector)
{
    Vector a = createVector(4.0, 0.0, 0.0);
    Vector a_n = normalizeVector(a);
    Vector b = createVector(1.0, 2.0, 3.0);
    Vector b_n = normalizeVector(b);
    Vector c = createVector(1 / std::sqrt(14.0), 1 / std::sqrt(14.0), 1 / std::sqrt(14.0));
    Vector c_n = normalizeVector(c);

    EXPECT_TRUE(a_n == Vector(1.0, 0.0, 0.0));
    EXPECT_TRUE(b_n == Vector(1.0 / std::sqrt(14.0), 2.0 / std::sqrt(14.0), 3.0 / std::sqrt(14.0)));
    EXPECT_DOUBLE_EQ(getVectorMagnitude(c_n), 1.0);
}

TEST(VectorOperations, VectorsDotProduct)
{
    Vector a = createVector(1.0, 2.0, 3.0);
    Vector b = createVector(2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(dotProduct(a, b), 20.0);
}

TEST(VectorOperations, VectorsCrossProduct)
{
    Vector a = createVector(1.0, 2.0, 3.0);
    Vector b = createVector(2.0, 3.0, 4.0);

    Vector a_first = crossProduct(a, b);

    EXPECT_TRUE(a_first == Vector(-1.0, 2.0, -1.0));

    Vector b_first = crossProduct(b, a);

    EXPECT_TRUE(b_first == Vector(1.0, -2.0, 1.0));
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
    Vector n = createVector(sqrt(2) / 2, sqrt(2) / 2, 0);
    Vector reflection = reflect(a, n);

    EXPECT_EQ(reflection, createVector(1, 0, 0));
}

// ------------------------------------------------------
// Utilities

TEST(Utilities, Equality)
{
    // 1. Exact equality
    EXPECT_TRUE(equal(1.0, 1.0));
    EXPECT_TRUE(equal(-5.5, -5.5));

    // 2. Near equality (within epsilon)
    EXPECT_TRUE(equal(1.000001, 1.000002));
    EXPECT_TRUE(equal(0.0, 0.0000001));

    // 3. Boundary case (Exactly EPSILON apart)
    EXPECT_FALSE(equal(1.0, 1.00001)); 

    // 4. Far apart
    EXPECT_FALSE(equal(1.0, 1.1));
    EXPECT_FALSE(equal(1.0, -1.0));
}