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
    A(0,0) = 1.0f;     A(0,1) = 2.0f;     A(0,2) = 3.0f;     A(0,3) = 4.0f;
    A(1,0) = 5.5f;     A(1,1) = 6.5f;     A(1,2) = 7.5f;     A(1,3) = 8.5f;
    A(2,0) = 9.0f;     A(2,1) = 10.0f;    A(2,2) = 11.0f;    A(2,3) = 12.0f;
    A(3,0) = 13.5f;    A(3,1) = 14.5f;    A(3,2) = 15.5f;    A(3,3) = 16.5f;

    matrix<4> B;
    B(0,0) = 2.0f;     B(0,1) = 3.0f;     B(0,2) = 4.0f;     B(0,3) = 5.0f;
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

// ---------------------------------------------------
// Matrix Operations

TEST(MatrixOperation, MultiplyTwo4x4Matrices)
{
    matrix<4> A;
    A(0,0) = 1.0f;     A(0,1) = 2.0f;     A(0,2) = 3.0f;     A(0,3) = 4.0f;
    A(1,0) = 5.0f;     A(1,1) = 6.0f;     A(1,2) = 7.0f;     A(1,3) = 8.0f;
    A(2,0) = 9.0f;     A(2,1) = 8.0f;     A(2,2) = 7.0f;     A(2,3) = 6.0f;
    A(3,0) = 5.0f;     A(3,1) = 4.0f;     A(3,2) = 3.0f;     A(3,3) = 2.0f;

    matrix<4> B;
    B(0,0) = -2.0f;     B(0,1) = 1.0f;     B(0,2) = 2.0f;     B(0,3) = 3.0f;
    B(1,0) = 3.0f;      B(1,1) = 2.0f;     B(1,2) = 1.0f;     B(1,3) = -1.0f;
    B(2,0) = 4.0f;      B(2,1) = 3.0f;     B(2,2) = 6.0f;     B(2,3) = 5.0f;
    B(3,0) = 1.0f;      B(3,1) = 2.0f;     B(3,2) = 7.0f;     B(3,3) = 8.0f;

    matrix<4> C = A * B;

    EXPECT_FLOAT_EQ(C(0,0), 20.0f);
    EXPECT_FLOAT_EQ(C(0,1), 22.0f);
    EXPECT_FLOAT_EQ(C(0,2), 50.0f);
    EXPECT_FLOAT_EQ(C(0,3), 48.0f);

    EXPECT_FLOAT_EQ(C(1,0), 44.0f);
    EXPECT_FLOAT_EQ(C(1,1), 54.0f);
    EXPECT_FLOAT_EQ(C(1,2), 114.0f);
    EXPECT_FLOAT_EQ(C(1,3), 108.0f);

	EXPECT_FLOAT_EQ(C(2,0), 40.0f);
    EXPECT_FLOAT_EQ(C(2,1), 58.0f);
    EXPECT_FLOAT_EQ(C(2,2), 110.0f);
    EXPECT_FLOAT_EQ(C(2,3), 102.0f);

	EXPECT_FLOAT_EQ(C(3,0), 16.0f);
    EXPECT_FLOAT_EQ(C(3,1), 26.0f);
    EXPECT_FLOAT_EQ(C(3,2), 46.0f);
    EXPECT_FLOAT_EQ(C(3,3), 42.0f);
}

TEST(MatrixOperation, Multiply4x4MatrixBy4x4IdentityMatrix)
{
    matrix<4> A;
    A(0,0) = 1.0f;     A(0,1) = 2.0f;     A(0,2) = 3.0f;     A(0,3) = 4.0f;
    A(1,0) = 1.0f;     A(1,1) = 2.0f;     A(1,2) = 4.0f;     A(1,3) = 8.0f;
    A(2,0) = 2.0f;     A(2,1) = 4.0f;     A(2,2) = 8.0f;     A(2,3) = 16.0f;
    A(3,0) = 4.0f;     A(3,1) = 8.0f;     A(3,2) = 16.0f;    A(3,3) = 32.0f;

    matrix<4> B;
    B(0,0) = 1.0f;      B(0,1) = 0.0f;     B(0,2) = 0.0f;     B(0,3) = 0.0f;
    B(1,0) = 0.0f;      B(1,1) = 1.0f;     B(1,2) = 0.0f;     B(1,3) = 0.0f;
    B(2,0) = 0.0f;      B(2,1) = 0.0f;     B(2,2) = 1.0f;     B(2,3) = 0.0f;
    B(3,0) = 0.0f;      B(3,1) = 0.0f;     B(3,2) = 0.0f;     B(3,3) = 1.0f;

    // Identity
    // | 1    | 0    | 0    | 0    |
    // | 0    | 1    | 0    | 0    |
    // | 0    | 0    | 1    | 0    |
    // | 0    | 0    | 0    | 1    |

    matrix<4> C = A * B;

    EXPECT_FLOAT_EQ(C(0,0), 1.0f);
    EXPECT_FLOAT_EQ(C(0,1), 2.0f);
    EXPECT_FLOAT_EQ(C(0,2), 3.0f);
    EXPECT_FLOAT_EQ(C(0,3), 4.0f);

    EXPECT_FLOAT_EQ(C(1,0), 1.0f);
    EXPECT_FLOAT_EQ(C(1,1), 2.0f);
    EXPECT_FLOAT_EQ(C(1,2), 4.0f);
    EXPECT_FLOAT_EQ(C(1,3), 8.0f);

	EXPECT_FLOAT_EQ(C(2,0), 2.0f);
    EXPECT_FLOAT_EQ(C(2,1), 4.0f);
    EXPECT_FLOAT_EQ(C(2,2), 8.0f);
    EXPECT_FLOAT_EQ(C(2,3), 16.0f);

	EXPECT_FLOAT_EQ(C(3,0), 4.0f);
    EXPECT_FLOAT_EQ(C(3,1), 8.0f);
    EXPECT_FLOAT_EQ(C(3,2), 16.0f);
    EXPECT_FLOAT_EQ(C(3,3), 32.0f);
}

TEST(MatrixOperation, Multiply4x4MatrixByTuple)
{
    matrix<4> A;
    A(0,0) = 1.0f;     A(0,1) = 2.0f;     A(0,2) = 3.0f;     A(0,3) = 4.0f;
    A(1,0) = 2.0f;     A(1,1) = 4.0f;     A(1,2) = 4.0f;     A(1,3) = 2.0f;
    A(2,0) = 8.0f;     A(2,1) = 6.0f;     A(2,2) = 4.0f;     A(2,3) = 1.0f;
    A(3,0) = 0.0f;     A(3,1) = 0.0f;     A(3,2) = 0.0f;     A(3,3) = 1.0f;

    tuple b(1.0, 2.0, 3.0, 1.0);

    tuple c(A * b);

    EXPECT_TRUE(c == tuple(18.0f, 24.0f, 33.0f, 1.0f));
}

TEST(MatrixOperation, Transpose4x4Matrix)
{
    matrix<4> A;
    A(0,0) = 0.0f;     A(0,1) = 9.0f;     A(0,2) = 3.0f;     A(0,3) = 0.0f;
    A(1,0) = 9.0f;     A(1,1) = 8.0f;     A(1,2) = 0.0f;     A(1,3) = 8.0f;
    A(2,0) = 1.0f;     A(2,1) = 8.0f;     A(2,2) = 5.0f;     A(2,3) = 3.0f;
    A(3,0) = 0.0f;     A(3,1) = 0.0f;     A(3,2) = 5.0f;     A(3,3) = 8.0f;

    // Transpose
    // | 0 | 9 | 3 | 0 |      | 0 | 9 | 1 | 0 |
    // | 9 | 8 | 0 | 8 |  =>  | 9 | 8 | 8 | 0 |
    // | 1 | 8 | 5 | 3 |      | 3 | 0 | 5 | 5 |
    // | 0 | 0 | 5 | 8 |      | 0 | 8 | 3 | 8 |

    matrix<4> B = transposeMatrix(A);

    EXPECT_FLOAT_EQ(B(0,0), 0.0f);
    EXPECT_FLOAT_EQ(B(0,1), 9.0f);
    EXPECT_FLOAT_EQ(B(0,2), 1.0f);
    EXPECT_FLOAT_EQ(B(0,3), 0.0f);

    EXPECT_FLOAT_EQ(B(1,0), 9.0f);
    EXPECT_FLOAT_EQ(B(1,1), 8.0f);
    EXPECT_FLOAT_EQ(B(1,2), 8.0f);
    EXPECT_FLOAT_EQ(B(1,3), 0.0f);

    EXPECT_FLOAT_EQ(B(2,0), 3.0f);
    EXPECT_FLOAT_EQ(B(2,1), 0.0f);
    EXPECT_FLOAT_EQ(B(2,2), 5.0f);
    EXPECT_FLOAT_EQ(B(2,3), 5.0f);

    EXPECT_FLOAT_EQ(B(3,0), 0.0f);
    EXPECT_FLOAT_EQ(B(3,1), 8.0f);
    EXPECT_FLOAT_EQ(B(3,2), 3.0f);
    EXPECT_FLOAT_EQ(B(3,3), 8.0f);
}

TEST(MatrixOperation, Transpose4x4IdentityMatrix)
{
    // Define the 4x4 Identity Matrix
    matrix<4> I;
    I(0,0) = 1.0f; I(0,1) = 0.0f; I(0,2) = 0.0f; I(0,3) = 0.0f;
    I(1,0) = 0.0f; I(1,1) = 1.0f; I(1,2) = 0.0f; I(1,3) = 0.0f;
    I(2,0) = 0.0f; I(2,1) = 0.0f; I(2,2) = 1.0f; I(2,3) = 0.0f;
    I(3,0) = 0.0f; I(3,1) = 0.0f; I(3,2) = 0.0f; I(3,3) = 1.0f;

    matrix<4> result = transposeMatrix(I);

    EXPECT_TRUE(result == I);
}

TEST(MatrixOperation, Determinant2x2Matrix)
{
    matrix<2> A;
    A(0,0) = 1.0f;  A(0,1) = 5.0f;
    A(1,0) = -3.0f; A(1,1) = 2.0f;

    EXPECT_FLOAT_EQ(determinantMatrix(A), 17.0f);
}

TEST(MatrixOperation, Submatrix3x3Is2x2)
{
    matrix<3> A;
    A(0,0) = 1.0f;   A(0,1) = 5.0f;   A(0,2) = 0.0f;
    A(1,0) = -3.0f;  A(1,1) = 2.0f;   A(1,2) = 7.0f;
    A(2,0) = 0.0f;   A(2,1) = 6.0f;   A(2,2) = -3.0f;

    // Submatrix(A, 0, 2) - Remove row 0, col 2
    // |  1 |  5 |  0 |      | -3 |  2 |
    // | -3 |  2 |  7 |  =>  |  0 |  6 |
    // |  0 |  6 | -3 |

    matrix<2> B = submatrix(A, 0, 2);

    EXPECT_FLOAT_EQ(B(0,0), -3.0f);
    EXPECT_FLOAT_EQ(B(0,1), 2.0f);

    EXPECT_FLOAT_EQ(B(1,0), 0.0f);
    EXPECT_FLOAT_EQ(B(1,1), 6.0f);
}

TEST(MatrixOperation, Submatrix4x4Is3x3)
{
    matrix<4> A;
    A(0,0) = -6.0f;  A(0,1) = 1.0f;  A(0,2) = 1.0f;  A(0,3) = 6.0f;
    A(1,0) = -8.0f;  A(1,1) = 5.0f;  A(1,2) = 8.0f;  A(1,3) = 6.0f;
    A(2,0) = -1.0f;  A(2,1) = 0.0f;  A(2,2) = 8.0f;  A(2,3) = 2.0f;
    A(3,0) = -7.0f;  A(3,1) = 1.0f;  A(3,2) = -1.0f; A(3,3) = 1.0f;

    // Submatrix(A, 2, 1) - Remove row 2, col 1
    // | -6 |  1 |  1 |  6 |      | -6 |  1 |  6 |
    // | -8 |  5 |  8 |  6 |  =>  | -8 |  8 |  6 |
    // | -1 |  0 |  8 |  2 |      | -7 | -1 |  1 |
    // | -7 |  1 | -1 |  1 |

    matrix<3> B = submatrix(A, 2, 1);

    EXPECT_FLOAT_EQ(B(0,0), -6.0f);
    EXPECT_FLOAT_EQ(B(0,1), 1.0f);
    EXPECT_FLOAT_EQ(B(0,2), 6.0f);

    EXPECT_FLOAT_EQ(B(1,0), -8.0f);
    EXPECT_FLOAT_EQ(B(1,1), 8.0f);
    EXPECT_FLOAT_EQ(B(1,2), 6.0f);

    EXPECT_FLOAT_EQ(B(2,0), -7.0f);
    EXPECT_FLOAT_EQ(B(2,1), -1.0f);
    EXPECT_FLOAT_EQ(B(2,2), 1.0f);
}