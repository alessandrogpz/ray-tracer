#include <gtest/gtest.h>
#include "../includes/Matrix.hpp"
#include "../includes/Tuple.hpp"
#include "../includes/Utils.hpp"

// ---------------------------------------------------
// Matrix Creation

TEST(MatrixCreation, createMatrix2x2)
{
    matrix<2> m2;
    m2(0,0) = -3.0f;    m2(0,1) = 5.0f;
    m2(1,0) = 1.0f;     m2(1,1) = -2.0f;

    // | -3 |  5 |
    // |  1 | -2 |

    EXPECT_FLOAT_EQ(m2(0,0), -3.0f);
    EXPECT_FLOAT_EQ(m2(0,1), 5.0f);
    EXPECT_FLOAT_EQ(m2(1,0), 1.0f);
    EXPECT_FLOAT_EQ(m2(1,1), -2.0f);
}

TEST(MatrixCreation, createMatrix3x3)
{
    matrix<3> m3;
    m3(0,0) = -3.0f;    m3(0,1) = 5.0f;     m3(0,2) = 0.0f;
    m3(1,0) = 1.0f;     m3(1,1) = -2.0f;    m3(1,2) = -7.0f;
    m3(2,0) = 0.0f;     m3(2,1) = 1.0f;     m3(2,2) = 1.0f;

    // | -3 |  5 |  0 |
    // |  1 | -2 | -7 |
    // |  0 |  1 |  1 |

    EXPECT_FLOAT_EQ(m3(0,0), -3.0f);
    EXPECT_FLOAT_EQ(m3(0,1), 5.0f);
    EXPECT_FLOAT_EQ(m3(0,2), 0.0f);

    EXPECT_FLOAT_EQ(m3(1,0), 1.0f);
    EXPECT_FLOAT_EQ(m3(1,1), -2.0f);
    EXPECT_FLOAT_EQ(m3(1,2), -7.0f);

    EXPECT_FLOAT_EQ(m3(2,0), 0.0f);
    EXPECT_FLOAT_EQ(m3(2,1), 1.0f);
    EXPECT_FLOAT_EQ(m3(2,2), 1.0f);
}

TEST(MatrixCreation, createMatrix4x4)
{
	matrix<4> m4;
    m4(0,0) = 1.0f;     m4(0,1) = 2.0f;     m4(0,2) = 3.0f;     m4(0,3) = 4.0f;
    m4(1,0) = 5.5f;     m4(1,1) = 6.5f;     m4(1,2) = 7.5f;     m4(1,3) = 8.5f;
    m4(2,0) = 9.0f;     m4(2,1) = 10.0f;    m4(2,2) = 11.0f;    m4(2,3) = 12.0f;
    m4(3,0) = 13.5f;    m4(3,1) = 14.5f;    m4(3,2) = 15.5f;    m4(3,3) = 16.5f;

    // | 1    | 2    | 3    | 4    |
    // | 5.5  | 6.5  | 7.5  | 8.5  |
    // | 9    | 10   | 11   | 12   |
    // | 13.5 | 14.5 | 15.5 | 16.5 |


    EXPECT_FLOAT_EQ(m4(0,0), 1.0f);
    EXPECT_FLOAT_EQ(m4(0,1), 2.0f);
    EXPECT_FLOAT_EQ(m4(0,2), 3.0f);
    EXPECT_FLOAT_EQ(m4(0,3), 4.0f);

    EXPECT_FLOAT_EQ(m4(1,0), 5.5f);
    EXPECT_FLOAT_EQ(m4(1,1), 6.5f);
    EXPECT_FLOAT_EQ(m4(1,2), 7.5f);
    EXPECT_FLOAT_EQ(m4(1,3), 8.5f);

	EXPECT_FLOAT_EQ(m4(2,0), 9.0f);
    EXPECT_FLOAT_EQ(m4(2,1), 10.0f);
    EXPECT_FLOAT_EQ(m4(2,2), 11.0f);
    EXPECT_FLOAT_EQ(m4(2,3), 12.0f);

	EXPECT_FLOAT_EQ(m4(3,0), 13.5f);
    EXPECT_FLOAT_EQ(m4(3,1), 14.5f);
    EXPECT_FLOAT_EQ(m4(3,2), 15.5f);
    EXPECT_FLOAT_EQ(m4(3,3), 16.5f);
}

// ---------------------------------------------------
// Matrix Comparison

TEST(MatrixComparison, identicalMatricesAreEqual)
{  
    matrix<4> A;
    A(0,0) = 1.0f;     A(0,1) = 2.0f;     A(0,2) = 3.0f;     A(0,3) = 4.0f;
    A(1,0) = 5.5f;     A(1,1) = 6.5f;     A(1,2) = 7.5f;     A(1,3) = 8.5f;
    A(2,0) = 9.0f;     A(2,1) = 10.0f;    A(2,2) = 11.0f;    A(2,3) = 12.0f;
    A(3,0) = 13.5f;    A(3,1) = 14.5f;    A(3,2) = 15.5f;    A(3,3) = 16.5f;

    matrix<4> B;
    B(0,0) = 1.0f;     B(0,1) = 2.0f;     B(0,2) = 3.0f;     B(0,3) = 4.0f;
    B(1,0) = 5.5f;     B(1,1) = 6.5f;     B(1,2) = 7.5f;     B(1,3) = 8.5f;
    B(2,0) = 9.0f;     B(2,1) = 10.0f;    B(2,2) = 11.0f;    B(2,3) = 12.0f;
    B(3,0) = 13.5f;    B(3,1) = 14.5f;    B(3,2) = 15.5f;    B(3,3) = 16.5f;

    EXPECT_TRUE(A == B);
}

TEST(MatrixComparison, verySimilarMatricesAreEqual)
{
    matrix<2> A;
    A(0,0) = 1.000000f;
    A(1,1) = 1.000000f;

    matrix<2> B;
    // Difference is 0.000001, which is smaller than 0.00001 EPSILON
    B(0,0) = 1.000001f; 
    B(1,1) = 0.999999f;

    EXPECT_TRUE(A == B);
}

TEST(MatrixComparison, differentMatricesAreNotEqual)
{
    matrix<4> A;
    A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f; A(0,3) = 4.0f;
    A(1,0) = 5.5f;     A(1,1) = 6.5f;     A(1,2) = 7.5f;     A(1,3) = 8.5f;
    A(2,0) = 9.0f;     A(2,1) = 10.0f;    A(2,2) = 11.0f;    A(2,3) = 12.0f;
    A(3,0) = 13.5f;    A(3,1) = 14.5f;    A(3,2) = 15.5f;    A(3,3) = 16.5f;

    matrix<4> B;
    B(0,0) = 2.0f; B(0,1) = 3.0f; B(0,2) = 4.0f; B(0,3) = 5.0f;
    B(1,0) = 5.5f;     B(1,1) = 6.5f;     B(1,2) = 7.5f;     B(1,3) = 8.5f;
    B(2,0) = 9.0f;     B(2,1) = 10.0f;    B(2,2) = 11.0f;    B(2,3) = 12.0f;
    B(3,0) = 13.5f;    B(3,1) = 14.5f;    B(3,2) = 15.5f;    B(3,3) = 16.5f;

    EXPECT_FALSE(A == B);
    EXPECT_TRUE(A != B);
}

TEST(MatrixComparison, slightlyDifferentOutsideEpsilonAreNotEqual)
{
    matrix<2> A;
    A(0,0) = 1.0f;

    matrix<2> B;
    // Difference is 0.0001, which is larger than 0.00001 EPSILON
    B(0,0) = 1.0001f; 

    EXPECT_FALSE(A == B);
}