#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

// ---------------------------------------------------
// Matrix Creation

TEST(MatrixCreation, createMatrix2x2)
{
    matrix<2> m2;
    m2(0,0) = -3.0;    m2(0,1) = 5.0;
    m2(1,0) = 1.0;     m2(1,1) = -2.0;

    // | -3 |  5 |
    // |  1 | -2 |

    EXPECT_DOUBLE_EQ(m2(0,0), -3.0);
    EXPECT_DOUBLE_EQ(m2(0,1), 5.0);
    EXPECT_DOUBLE_EQ(m2(1,0), 1.0);
    EXPECT_DOUBLE_EQ(m2(1,1), -2.0);
}

TEST(MatrixCreation, createMatrix3x3)
{
    matrix<3> m3;
    m3(0,0) = -3.0;    m3(0,1) = 5.0;     m3(0,2) = 0.0;
    m3(1,0) = 1.0;     m3(1,1) = -2.0;    m3(1,2) = -7.0;
    m3(2,0) = 0.0;     m3(2,1) = 1.0;     m3(2,2) = 1.0;

    // | -3 |  5 |  0 |
    // |  1 | -2 | -7 |
    // |  0 |  1 |  1 |

    EXPECT_DOUBLE_EQ(m3(0,0), -3.0);
    EXPECT_DOUBLE_EQ(m3(0,1), 5.0);
    EXPECT_DOUBLE_EQ(m3(0,2), 0.0);

    EXPECT_DOUBLE_EQ(m3(1,0), 1.0);
    EXPECT_DOUBLE_EQ(m3(1,1), -2.0);
    EXPECT_DOUBLE_EQ(m3(1,2), -7.0);

    EXPECT_DOUBLE_EQ(m3(2,0), 0.0);
    EXPECT_DOUBLE_EQ(m3(2,1), 1.0);
    EXPECT_DOUBLE_EQ(m3(2,2), 1.0);
}

TEST(MatrixCreation, createMatrix4x4)
{
	matrix<4> m4;
    m4(0,0) = 1.0;     m4(0,1) = 2.0;     m4(0,2) = 3.0;     m4(0,3) = 4.0;
    m4(1,0) = 5.5;     m4(1,1) = 6.5;     m4(1,2) = 7.5;     m4(1,3) = 8.5;
    m4(2,0) = 9.0;     m4(2,1) = 10.0;    m4(2,2) = 11.0;    m4(2,3) = 12.0;
    m4(3,0) = 13.5;    m4(3,1) = 14.5;    m4(3,2) = 15.5;    m4(3,3) = 16.5;

    // | 1    | 2    | 3    | 4    |
    // | 5.5  | 6.5  | 7.5  | 8.5  |
    // | 9    | 10   | 11   | 12   |
    // | 13.5 | 14.5 | 15.5 | 16.5 |


    EXPECT_DOUBLE_EQ(m4(0,0), 1.0);
    EXPECT_DOUBLE_EQ(m4(0,1), 2.0);
    EXPECT_DOUBLE_EQ(m4(0,2), 3.0);
    EXPECT_DOUBLE_EQ(m4(0,3), 4.0);

    EXPECT_DOUBLE_EQ(m4(1,0), 5.5);
    EXPECT_DOUBLE_EQ(m4(1,1), 6.5);
    EXPECT_DOUBLE_EQ(m4(1,2), 7.5);
    EXPECT_DOUBLE_EQ(m4(1,3), 8.5);

	EXPECT_DOUBLE_EQ(m4(2,0), 9.0);
    EXPECT_DOUBLE_EQ(m4(2,1), 10.0);
    EXPECT_DOUBLE_EQ(m4(2,2), 11.0);
    EXPECT_DOUBLE_EQ(m4(2,3), 12.0);

	EXPECT_DOUBLE_EQ(m4(3,0), 13.5);
    EXPECT_DOUBLE_EQ(m4(3,1), 14.5);
    EXPECT_DOUBLE_EQ(m4(3,2), 15.5);
    EXPECT_DOUBLE_EQ(m4(3,3), 16.5);
}

// ---------------------------------------------------
// Matrix Comparison

TEST(MatrixComparison, identicalMatricesAreEqual)
{  
    matrix<4> A;
    A(0,0) = 1.0;     A(0,1) = 2.0;     A(0,2) = 3.0;     A(0,3) = 4.0;
    A(1,0) = 5.5;     A(1,1) = 6.5;     A(1,2) = 7.5;     A(1,3) = 8.5;
    A(2,0) = 9.0;     A(2,1) = 10.0;    A(2,2) = 11.0;    A(2,3) = 12.0;
    A(3,0) = 13.5;    A(3,1) = 14.5;    A(3,2) = 15.5;    A(3,3) = 16.5;

    matrix<4> B;
    B(0,0) = 1.0;     B(0,1) = 2.0;     B(0,2) = 3.0;     B(0,3) = 4.0;
    B(1,0) = 5.5;     B(1,1) = 6.5;     B(1,2) = 7.5;     B(1,3) = 8.5;
    B(2,0) = 9.0;     B(2,1) = 10.0;    B(2,2) = 11.0;    B(2,3) = 12.0;
    B(3,0) = 13.5;    B(3,1) = 14.5;    B(3,2) = 15.5;    B(3,3) = 16.5;

    EXPECT_TRUE(A == B);
}

TEST(MatrixComparison, verySimilarMatricesAreEqual)
{
    matrix<2> A;
    A(0,0) = 1.000000;
    A(1,1) = 1.000000;

    matrix<2> B;
    // Difference is 0.000001, which is smaller than 0.00001 EPSILON
    B(0,0) = 1.000001; 
    B(1,1) = 0.999999;

    EXPECT_TRUE(A == B);
}

TEST(MatrixComparison, differentMatricesAreNotEqual)
{
    matrix<4> A;
    A(0,0) = 1.0;     A(0,1) = 2.0;     A(0,2) = 3.0;     A(0,3) = 4.0;
    A(1,0) = 5.5;     A(1,1) = 6.5;     A(1,2) = 7.5;     A(1,3) = 8.5;
    A(2,0) = 9.0;     A(2,1) = 10.0;    A(2,2) = 11.0;    A(2,3) = 12.0;
    A(3,0) = 13.5;    A(3,1) = 14.5;    A(3,2) = 15.5;    A(3,3) = 16.5;

    matrix<4> B;
    B(0,0) = 2.0;     B(0,1) = 3.0;     B(0,2) = 4.0;     B(0,3) = 5.0;
    B(1,0) = 5.5;     B(1,1) = 6.5;     B(1,2) = 7.5;     B(1,3) = 8.5;
    B(2,0) = 9.0;     B(2,1) = 10.0;    B(2,2) = 11.0;    B(2,3) = 12.0;
    B(3,0) = 13.5;    B(3,1) = 14.5;    B(3,2) = 15.5;    B(3,3) = 16.5;

    EXPECT_FALSE(A == B);
    EXPECT_TRUE(A != B);
}

TEST(MatrixComparison, slightlyDifferentOutsideEpsilonAreNotEqual)
{
    matrix<2> A;
    A(0,0) = 1.0;

    matrix<2> B;
    // Difference is 0.0001, which is larger than 0.00001 EPSILON
    B(0,0) = 1.0001; 

    EXPECT_FALSE(A == B);
}

// ---------------------------------------------------
// Matrix Operations

TEST(MatrixOperation, MultiplyTwo4x4Matrices)
{
    matrix<4> A;
    A(0,0) = 1.0;     A(0,1) = 2.0;     A(0,2) = 3.0;     A(0,3) = 4.0;
    A(1,0) = 5.0;     A(1,1) = 6.0;     A(1,2) = 7.0;     A(1,3) = 8.0;
    A(2,0) = 9.0;     A(2,1) = 8.0;     A(2,2) = 7.0;     A(2,3) = 6.0;
    A(3,0) = 5.0;     A(3,1) = 4.0;     A(3,2) = 3.0;     A(3,3) = 2.0;

    matrix<4> B;
    B(0,0) = -2.0;     B(0,1) = 1.0;     B(0,2) = 2.0;     B(0,3) = 3.0;
    B(1,0) = 3.0;      B(1,1) = 2.0;     B(1,2) = 1.0;     B(1,3) = -1.0;
    B(2,0) = 4.0;      B(2,1) = 3.0;     B(2,2) = 6.0;     B(2,3) = 5.0;
    B(3,0) = 1.0;      B(3,1) = 2.0;     B(3,2) = 7.0;     B(3,3) = 8.0;

    matrix<4> C = A * B;

    EXPECT_DOUBLE_EQ(C(0,0), 20.0);
    EXPECT_DOUBLE_EQ(C(0,1), 22.0);
    EXPECT_DOUBLE_EQ(C(0,2), 50.0);
    EXPECT_DOUBLE_EQ(C(0,3), 48.0);

    EXPECT_DOUBLE_EQ(C(1,0), 44.0);
    EXPECT_DOUBLE_EQ(C(1,1), 54.0);
    EXPECT_DOUBLE_EQ(C(1,2), 114.0);
    EXPECT_DOUBLE_EQ(C(1,3), 108.0);

	EXPECT_DOUBLE_EQ(C(2,0), 40.0);
    EXPECT_DOUBLE_EQ(C(2,1), 58.0);
    EXPECT_DOUBLE_EQ(C(2,2), 110.0);
    EXPECT_DOUBLE_EQ(C(2,3), 102.0);

	EXPECT_DOUBLE_EQ(C(3,0), 16.0);
    EXPECT_DOUBLE_EQ(C(3,1), 26.0);
    EXPECT_DOUBLE_EQ(C(3,2), 46.0);
    EXPECT_DOUBLE_EQ(C(3,3), 42.0);
}

TEST(MatrixOperation, Multiply4x4MatrixBy4x4IdentityMatrix)
{
    matrix<4> A;
    A(0,0) = 1.0;     A(0,1) = 2.0;     A(0,2) = 3.0;     A(0,3) = 4.0;
    A(1,0) = 1.0;     A(1,1) = 2.0;     A(1,2) = 4.0;     A(1,3) = 8.0;
    A(2,0) = 2.0;     A(2,1) = 4.0;     A(2,2) = 8.0;     A(2,3) = 16.0;
    A(3,0) = 4.0;     A(3,1) = 8.0;     A(3,2) = 16.0;    A(3,3) = 32.0;

    matrix<4> B;
    B(0,0) = 1.0;      B(0,1) = 0.0;     B(0,2) = 0.0;     B(0,3) = 0.0;
    B(1,0) = 0.0;      B(1,1) = 1.0;     B(1,2) = 0.0;     B(1,3) = 0.0;
    B(2,0) = 0.0;      B(2,1) = 0.0;     B(2,2) = 1.0;     B(2,3) = 0.0;
    B(3,0) = 0.0;      B(3,1) = 0.0;     B(3,2) = 0.0;     B(3,3) = 1.0;

    // Identity
    // | 1    | 0    | 0    | 0    |
    // | 0    | 1    | 0    | 0    |
    // | 0    | 0    | 1    | 0    |
    // | 0    | 0    | 0    | 1    |

    matrix<4> C = A * B;

    EXPECT_DOUBLE_EQ(C(0,0), 1.0);
    EXPECT_DOUBLE_EQ(C(0,1), 2.0);
    EXPECT_DOUBLE_EQ(C(0,2), 3.0);
    EXPECT_DOUBLE_EQ(C(0,3), 4.0);

    EXPECT_DOUBLE_EQ(C(1,0), 1.0);
    EXPECT_DOUBLE_EQ(C(1,1), 2.0);
    EXPECT_DOUBLE_EQ(C(1,2), 4.0);
    EXPECT_DOUBLE_EQ(C(1,3), 8.0);

	EXPECT_DOUBLE_EQ(C(2,0), 2.0);
    EXPECT_DOUBLE_EQ(C(2,1), 4.0);
    EXPECT_DOUBLE_EQ(C(2,2), 8.0);
    EXPECT_DOUBLE_EQ(C(2,3), 16.0);

	EXPECT_DOUBLE_EQ(C(3,0), 4.0);
    EXPECT_DOUBLE_EQ(C(3,1), 8.0);
    EXPECT_DOUBLE_EQ(C(3,2), 16.0);
    EXPECT_DOUBLE_EQ(C(3,3), 32.0);
}

TEST(MatrixOperation, Multiply4x4MatrixByTuple)
{
    matrix<4> A;
    A(0,0) = 1.0;     A(0,1) = 2.0;     A(0,2) = 3.0;     A(0,3) = 4.0;
    A(1,0) = 2.0;     A(1,1) = 4.0;     A(1,2) = 4.0;     A(1,3) = 2.0;
    A(2,0) = 8.0;     A(2,1) = 6.0;     A(2,2) = 4.0;     A(2,3) = 1.0;
    A(3,0) = 0.0;     A(3,1) = 0.0;     A(3,2) = 0.0;     A(3,3) = 1.0;

    tuple b(1.0, 2.0, 3.0, 1.0);

    tuple c(A * b);

    EXPECT_TRUE(c == tuple(18.0, 24.0, 33.0, 1.0));
}

TEST(MatrixOperation, Transpose4x4Matrix)
{
    matrix<4> A;
    A(0,0) = 0.0;     A(0,1) = 9.0;     A(0,2) = 3.0;     A(0,3) = 0.0;
    A(1,0) = 9.0;     A(1,1) = 8.0;     A(1,2) = 0.0;     A(1,3) = 8.0;
    A(2,0) = 1.0;     A(2,1) = 8.0;     A(2,2) = 5.0;     A(2,3) = 3.0;
    A(3,0) = 0.0;     A(3,1) = 0.0;     A(3,2) = 5.0;     A(3,3) = 8.0;

    // Transpose
    // | 0 | 9 | 3 | 0 |      | 0 | 9 | 1 | 0 |
    // | 9 | 8 | 0 | 8 |  =>  | 9 | 8 | 8 | 0 |
    // | 1 | 8 | 5 | 3 |      | 3 | 0 | 5 | 5 |
    // | 0 | 0 | 5 | 8 |      | 0 | 8 | 3 | 8 |

    matrix<4> B = A.transpose();

    EXPECT_DOUBLE_EQ(B(0,0), 0.0);
    EXPECT_DOUBLE_EQ(B(0,1), 9.0);
    EXPECT_DOUBLE_EQ(B(0,2), 1.0);
    EXPECT_DOUBLE_EQ(B(0,3), 0.0);

    EXPECT_DOUBLE_EQ(B(1,0), 9.0);
    EXPECT_DOUBLE_EQ(B(1,1), 8.0);
    EXPECT_DOUBLE_EQ(B(1,2), 8.0);
    EXPECT_DOUBLE_EQ(B(1,3), 0.0);

    EXPECT_DOUBLE_EQ(B(2,0), 3.0);
    EXPECT_DOUBLE_EQ(B(2,1), 0.0);
    EXPECT_DOUBLE_EQ(B(2,2), 5.0);
    EXPECT_DOUBLE_EQ(B(2,3), 5.0);

    EXPECT_DOUBLE_EQ(B(3,0), 0.0);
    EXPECT_DOUBLE_EQ(B(3,1), 8.0);
    EXPECT_DOUBLE_EQ(B(3,2), 3.0);
    EXPECT_DOUBLE_EQ(B(3,3), 8.0);
}

TEST(MatrixOperation, Transpose4x4IdentityMatrix)
{
    // Define the 4x4 Identity Matrix
    matrix<4> I;
    I(0,0) = 1.0; I(0,1) = 0.0; I(0,2) = 0.0; I(0,3) = 0.0;
    I(1,0) = 0.0; I(1,1) = 1.0; I(1,2) = 0.0; I(1,3) = 0.0;
    I(2,0) = 0.0; I(2,1) = 0.0; I(2,2) = 1.0; I(2,3) = 0.0;
    I(3,0) = 0.0; I(3,1) = 0.0; I(3,2) = 0.0; I(3,3) = 1.0;

    matrix<4> result = I.transpose();

    EXPECT_TRUE(result == I);
}

TEST(MatrixOperation, Determinant2x2Matrix)
{
    matrix<2> A;
    A(0,0) = 1.0;  A(0,1) = 5.0;
    A(1,0) = -3.0; A(1,1) = 2.0;

    EXPECT_DOUBLE_EQ(A.determinant(), 17.0);
}

TEST(MatrixOperation, Submatrix3x3Is2x2)
{
    matrix<3> A;
    A(0,0) = 1.0;   A(0,1) = 5.0;   A(0,2) = 0.0;
    A(1,0) = -3.0;  A(1,1) = 2.0;   A(1,2) = 7.0;
    A(2,0) = 0.0;   A(2,1) = 6.0;   A(2,2) = -3.0;

    // Submatrix(A, 0, 2) - Remove row 0, col 2
    // |  1 |  5 |  0 |      | -3 |  2 |
    // | -3 |  2 |  7 |  =>  |  0 |  6 |
    // |  0 |  6 | -3 |

    matrix<2> B = A.submatrix(0, 2);

    EXPECT_DOUBLE_EQ(B(0,0), -3.0);
    EXPECT_DOUBLE_EQ(B(0,1), 2.0);

    EXPECT_DOUBLE_EQ(B(1,0), 0.0);
    EXPECT_DOUBLE_EQ(B(1,1), 6.0);
}

TEST(MatrixOperation, Submatrix4x4Is3x3)
{
    matrix<4> A;
    A(0,0) = -6.0;  A(0,1) = 1.0;  A(0,2) = 1.0;  A(0,3) = 6.0;
    A(1,0) = -8.0;  A(1,1) = 5.0;  A(1,2) = 8.0;  A(1,3) = 6.0;
    A(2,0) = -1.0;  A(2,1) = 0.0;  A(2,2) = 8.0;  A(2,3) = 2.0;
    A(3,0) = -7.0;  A(3,1) = 1.0;  A(3,2) = -1.0; A(3,3) = 1.0;

    // Submatrix(A, 2, 1) - Remove row 2, col 1
    // | -6 |  1 |  1 |  6 |      | -6 |  1 |  6 |
    // | -8 |  5 |  8 |  6 |  =>  | -8 |  8 |  6 |
    // | -1 |  0 |  8 |  2 |      | -7 | -1 |  1 |
    // | -7 |  1 | -1 |  1 |

    matrix<3> B = A.submatrix(2, 1);

    EXPECT_DOUBLE_EQ(B(0,0), -6.0);
    EXPECT_DOUBLE_EQ(B(0,1), 1.0);
    EXPECT_DOUBLE_EQ(B(0,2), 6.0);

    EXPECT_DOUBLE_EQ(B(1,0), -8.0);
    EXPECT_DOUBLE_EQ(B(1,1), 8.0);
    EXPECT_DOUBLE_EQ(B(1,2), 6.0);

    EXPECT_DOUBLE_EQ(B(2,0), -7.0);
    EXPECT_DOUBLE_EQ(B(2,1), -1.0);
    EXPECT_DOUBLE_EQ(B(2,2), 1.0);
}

TEST(MatrixOperation, minor3x3matrix)
{
    matrix<3> A;
    A(0,0) = 3.0;  A(0,1) = 5.0;   A(0,2) = 0.0; 
    A(1,0) = 2.0;  A(1,1) = -1.0;  A(1,2) = -7.0;  
    A(2,0) = 6.0;  A(2,1) = -1.0;  A(2,2) = 5.0; 

    // |  3 |  5 |  0 |
    // |  2 | -1 | -7 |
    // |  6 | -1 |  5 |

    EXPECT_DOUBLE_EQ(A.minor(1, 0), 25.0);
}

TEST(MatrixOperation, CalculatingCofactor3x3Matrix)
{
    matrix<3> A;
    A(0,0) = 3.0;  A(0,1) = 5.0;   A(0,2) = 0.0;
    A(1,0) = 2.0;  A(1,1) = -1.0;  A(1,2) = -7.0;
    A(2,0) = 6.0;  A(2,1) = -1.0;  A(2,2) = 5.0;

    EXPECT_DOUBLE_EQ(A.minor(0, 0), -12.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), -12.0);
    
    EXPECT_DOUBLE_EQ(A.minor(1, 0), 25.0);
    EXPECT_DOUBLE_EQ(A.cofactor(1, 0), -25.0);
}

TEST(MatrixOperation, Determinant3x3Matrix)
{
    matrix<3> A;
    A(0,0) = 1.0;  A(0,1) = 2.0;  A(0,2) = 6.0;
    A(1,0) = -5.0; A(1,1) = 8.0;  A(1,2) = -4.0;
    A(2,0) = 2.0;  A(2,1) = 6.0;  A(2,2) = 4.0;

    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), 56.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 1), 12.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 2), -46.0);
    EXPECT_DOUBLE_EQ(A.determinant(), -196.0);
}

TEST(MatrixOperation, Determinant4x4Matrix)
{
    matrix<4> A;
    A(0,0) = -2.0; A(0,1) = -8.0; A(0,2) = 3.0;  A(0,3) = 5.0;
    A(1,0) = -3.0; A(1,1) = 1.0;  A(1,2) = 7.0;  A(1,3) = 3.0;
    A(2,0) = 1.0;  A(2,1) = 2.0;  A(2,2) = -9.0; A(2,3) = 6.0;
    A(3,0) = -6.0; A(3,1) = 7.0;  A(3,2) = 7.0;  A(3,3) = -9.0;

    EXPECT_DOUBLE_EQ(A.cofactor(0, 0), 690.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 1), 447.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 2), 210.0);
    EXPECT_DOUBLE_EQ(A.cofactor(0, 3), 51.0);
    EXPECT_DOUBLE_EQ(A.determinant(), -4071.0);
}

TEST(MatrixOperation, IsInvertible)
{
    matrix<4> A;
    A(0,0) = 6.0;  A(0,1) = 4.0;  A(0,2) = 4.0;  A(0,3) = 4.0;
    A(1,0) = 5.0;  A(1,1) = 5.0;  A(1,2) = 7.0;  A(1,3) = 6.0;
    A(2,0) = 4.0;  A(2,1) = -9.0; A(2,2) = 3.0;  A(2,3) = -7.0;
    A(3,0) = 9.0;  A(3,1) = 1.0;  A(3,2) = 7.0;  A(3,3) = -6.0;

    EXPECT_DOUBLE_EQ(A.determinant(), -2120.0);
    EXPECT_TRUE(A.isInvertible());
}

TEST(MatrixOperation, IsNotInvertible)
{
    matrix<4> A;
    A(0,0) = -4.0; A(0,1) = 2.0;  A(0,2) = -2.0; A(0,3) = -3.0;
    A(1,0) = 9.0;  A(1,1) = 6.0;  A(1,2) = 2.0;  A(1,3) = 6.0;
    A(2,0) = 0.0;  A(2,1) = -5.0; A(2,2) = 1.0;  A(2,3) = -5.0;
    A(3,0) = 0.0;  A(3,1) = 0.0;  A(3,2) = 0.0;  A(3,3) = 0.0;

    // Use EXPECT_NEAR or EXPECT_DOUBLE_EQ depending on how your double comparison is setup
    EXPECT_DOUBLE_EQ(A.determinant(), 0.0);
    EXPECT_FALSE(A.isInvertible());
}

TEST(MatrixOperation, CalculateInverse4x4)
{
    matrix<4> A;
    A(0,0) = -5.0; A(0,1) = 2.0;  A(0,2) = 6.0;  A(0,3) = -8.0;
    A(1,0) = 1.0;  A(1,1) = -5.0; A(1,2) = 1.0;  A(1,3) = 8.0;
    A(2,0) = 7.0;  A(2,1) = 7.0;  A(2,2) = -6.0; A(2,3) = -7.0;
    A(3,0) = 1.0;  A(3,1) = -3.0; A(3,2) = 7.0;  A(3,3) = 4.0;

    matrix<4> B = A.inverse();

    // Intermediate checks to verify the math steps
    EXPECT_DOUBLE_EQ(A.determinant(), 532.0);
    EXPECT_DOUBLE_EQ(A.cofactor(2, 3), -160.0);
    EXPECT_DOUBLE_EQ(B(3, 2), -160.0 / 532.0);
    EXPECT_DOUBLE_EQ(A.cofactor(3, 2), 105.0);
    EXPECT_DOUBLE_EQ(B(2, 3), 105.0 / 532.0);

    // Final matrix verification
    // Note: We use EXPECT_NEAR here with a tolerance of 0.00001 because 
    // the book's test case rounds the expected fractions to 5 decimal places.
    EXPECT_NEAR(B(0,0), 0.21805, 0.00001);
    EXPECT_NEAR(B(0,1), 0.45113, 0.00001);
    EXPECT_NEAR(B(0,2), 0.24060, 0.00001);
    EXPECT_NEAR(B(0,3), -0.04511, 0.00001);

    EXPECT_NEAR(B(1,0), -0.80827, 0.00001);
    EXPECT_NEAR(B(1,1), -1.45677, 0.00001);
    EXPECT_NEAR(B(1,2), -0.44361, 0.00001);
    EXPECT_NEAR(B(1,3), 0.52068, 0.00001);

    EXPECT_NEAR(B(2,0), -0.07895, 0.00001);
    EXPECT_NEAR(B(2,1), -0.22368, 0.00001);
    EXPECT_NEAR(B(2,2), -0.05263, 0.00001);
    EXPECT_NEAR(B(2,3), 0.19737, 0.00001);

    EXPECT_NEAR(B(3,0), -0.52256, 0.00001);
    EXPECT_NEAR(B(3,1), -0.81391, 0.00001);
    EXPECT_NEAR(B(3,2), -0.30075, 0.00001);
    EXPECT_NEAR(B(3,3), 0.30639, 0.00001);
}

TEST(MatrixOperation, CalculateInverseAnother4x4)
{
    matrix<4> A;
    A(0,0) = 8.0;  A(0,1) = -5.0; A(0,2) = 9.0;  A(0,3) = 2.0;
    A(1,0) = 7.0;  A(1,1) = 5.0;  A(1,2) = 6.0;  A(1,3) = 1.0;
    A(2,0) = -6.0; A(2,1) = 0.0;  A(2,2) = 9.0;  A(2,3) = 6.0;
    A(3,0) = -3.0; A(3,1) = 0.0;  A(3,2) = -9.0; A(3,3) = -4.0;

    matrix<4> B = A.inverse();

    EXPECT_NEAR(B(0,0), -0.15385, 0.00001);
    EXPECT_NEAR(B(0,1), -0.15385, 0.00001);
    EXPECT_NEAR(B(0,2), -0.28205, 0.00001);
    EXPECT_NEAR(B(0,3), -0.53846, 0.00001);

    EXPECT_NEAR(B(1,0), -0.07692, 0.00001);
    EXPECT_NEAR(B(1,1), 0.12308, 0.00001);
    EXPECT_NEAR(B(1,2), 0.02564, 0.00001);
    EXPECT_NEAR(B(1,3), 0.03077, 0.00001);

    EXPECT_NEAR(B(2,0), 0.35897, 0.00001);
    EXPECT_NEAR(B(2,1), 0.35897, 0.00001);
    EXPECT_NEAR(B(2,2), 0.43590, 0.00001);
    EXPECT_NEAR(B(2,3), 0.92308, 0.00001);

    EXPECT_NEAR(B(3,0), -0.69231, 0.00001);
    EXPECT_NEAR(B(3,1), -0.69231, 0.00001);
    EXPECT_NEAR(B(3,2), -0.76923, 0.00001);
    EXPECT_NEAR(B(3,3), -1.92308, 0.00001);
}

TEST(MatrixOperation, CalculateInverseThird4x4)
{
    matrix<4> A;
    A(0,0) = 9.0;  A(0,1) = 3.0;  A(0,2) = 0.0;  A(0,3) = 9.0;
    A(1,0) = -5.0; A(1,1) = -2.0; A(1,2) = -6.0; A(1,3) = -3.0;
    A(2,0) = -4.0; A(2,1) = 9.0;  A(2,2) = 6.0;  A(2,3) = 4.0;
    A(3,0) = -7.0; A(3,1) = 6.0;  A(3,2) = 6.0;  A(3,3) = 2.0;

    matrix<4> B = A.inverse();

    EXPECT_NEAR(B(0,0), -0.04074, 0.00001);
    EXPECT_NEAR(B(0,1), -0.07778, 0.00001);
    EXPECT_NEAR(B(0,2), 0.14444, 0.00001);
    EXPECT_NEAR(B(0,3), -0.22222, 0.00001);

    EXPECT_NEAR(B(1,0), -0.07778, 0.00001);
    EXPECT_NEAR(B(1,1), 0.03333, 0.00001);
    EXPECT_NEAR(B(1,2), 0.36667, 0.00001);
    EXPECT_NEAR(B(1,3), -0.33333, 0.00001);

    EXPECT_NEAR(B(2,0), -0.02901, 0.00001);
    EXPECT_NEAR(B(2,1), -0.14630, 0.00001);
    EXPECT_NEAR(B(2,2), -0.10926, 0.00001);
    EXPECT_NEAR(B(2,3), 0.12963, 0.00001);

    EXPECT_NEAR(B(3,0), 0.17778, 0.00001);
    EXPECT_NEAR(B(3,1), 0.06667, 0.00001);
    EXPECT_NEAR(B(3,2), -0.26667, 0.00001);
    EXPECT_NEAR(B(3,3), 0.33333, 0.00001);
}

TEST(MatrixOperation, MultiplyProductByInverse)
{
    matrix<4> A;
    A(0,0) = 3.0;  A(0,1) = -9.0; A(0,2) = 7.0;  A(0,3) = 3.0;
    A(1,0) = 3.0;  A(1,1) = -8.0; A(1,2) = 2.0;  A(1,3) = -9.0;
    A(2,0) = -4.0; A(2,1) = 4.0;  A(2,2) = 4.0;  A(2,3) = 1.0;
    A(3,0) = -6.0; A(3,1) = 5.0;  A(3,2) = -1.0; A(3,3) = 1.0;

    matrix<4> B;
    B(0,0) = 8.0;  B(0,1) = 2.0;  B(0,2) = 2.0;  B(0,3) = 2.0;
    B(1,0) = 3.0;  B(1,1) = -1.0; B(1,2) = 7.0;  B(1,3) = 0.0;
    B(2,0) = 7.0;  B(2,1) = 0.0;  B(2,2) = 5.0;  B(2,3) = 4.0;
    B(3,0) = 6.0;  B(3,1) = -2.0; B(3,2) = 0.0;  B(3,3) = 5.0;

    matrix<4> C = A * B;

    EXPECT_TRUE(C * B.inverse() == A);
}

TEST(MatrixOperation, AddTwoMatrices)
{
    matrix<2> A;
    A(0,0) = 1.0; A(0,1) = 2.0;
    A(1,0) = 3.0; A(1,1) = 4.0;

    matrix<2> B;
    B(0,0) = 5.0; B(0,1) = 6.0;
    B(1,0) = 7.0; B(1,1) = 8.0;

    matrix<2> C = A + B;

    EXPECT_DOUBLE_EQ(C(0,0), 6.0);
    EXPECT_DOUBLE_EQ(C(0,1), 8.0);
    EXPECT_DOUBLE_EQ(C(1,0), 10.0);
    EXPECT_DOUBLE_EQ(C(1,1), 12.0);
}

TEST(MatrixOperation, SubtractTwoMatrices)
{
    matrix<2> A;
    A(0,0) = 5.0; A(0,1) = 6.0;
    A(1,0) = 7.0; A(1,1) = 8.0;

    matrix<2> B;
    B(0,0) = 1.0; B(0,1) = 2.0;
    B(1,0) = 3.0; B(1,1) = 4.0;

    matrix<2> C = A - B;

    EXPECT_DOUBLE_EQ(C(0,0), 4.0);
    EXPECT_DOUBLE_EQ(C(0,1), 4.0);
    EXPECT_DOUBLE_EQ(C(1,0), 4.0);
    EXPECT_DOUBLE_EQ(C(1,1), 4.0);
}

TEST(MatrixOperation, MultiplyMatrixByScalar)
{
    matrix<2> A;
    A(0,0) = 1.0; A(0,1) = -2.0;
    A(1,0) = 3.0; A(1,1) = 4.0;

    matrix<2> B = A * 2.5;

    EXPECT_DOUBLE_EQ(B(0,0), 2.5);
    EXPECT_DOUBLE_EQ(B(0,1), -5.0);
    EXPECT_DOUBLE_EQ(B(1,0), 7.5);
    EXPECT_DOUBLE_EQ(B(1,1), 10.0);
}