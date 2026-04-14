#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../includes/Utils.hpp"
#include "../includes/Tuple.hpp"

template <int N>
struct matrix {
    float data[N][N];

    // Default constructor: Initializes all elements to 0
    matrix() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = 0.0f;
            }
        }
    }

    // Setter: m(row, col)
    float& operator()(int row, int col) {
        return data[row][col];
    }

    // Getter
    float operator()(int row, int col) const {
        return data[row][col];
    }

    // Overload for matrix comparison (==)
    friend bool operator==(const matrix<N>& a, const matrix<N>& b) {
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (!equal(a(r, c), b(r, c))) {
                    return false;
                }
            }
        }
        return true;
    }

    // Overload for matrix comparison (!=)
    friend bool operator!=(const matrix<N>& a, const matrix<N>& b) {
        return !(a == b);
    }

    // Overload for matrix multiplication
    friend matrix<N> operator*(const matrix<N>& a, const matrix<N>& b){
        matrix<N> result;

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                float sum = 0.0f;
                for (int k = 0; k < N; k++) {
                    sum += a(r, k) * b(k, c);
                }
                result(r, c) = sum;
            }
        }
        return result;
    }

    // Overload for matrix * tuple multiplication
    friend tuple operator*(const matrix<N>& m, const tuple& t) requires (N == 4) {
        float res[4] = {0, 0, 0, 0};

        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                res[r] += m(r, c) * t[c];
            }
        }

        return tuple(res[0], res[1], res[2], res[3]);
    }
};

template <int N>
matrix<N> transposeMatrix(const matrix<N>& m) {
    matrix<N> result;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            result(c, r) = m(r, c);
        }
    }
    return result;
}

template <int N>
float determinantMatrix(const matrix<N> &m) {

    if constexpr (N == 2)
    {
        return (m(0,0) * m(1,1)) - (m(0,1) * m(1,0));
    } else {
        float determinant = 0.0f;

        for (int col = 0; col < N; ++col) {
            determinant += m(0, col) * cofactor(m, 0, col);
        }

        return determinant;
    }
}

template <int N> requires (N >= 2)
matrix<N-1> submatrix(const matrix<N>& m, int skip_row, int skip_col)
{   
    matrix<N-1> result;

    int dest_row = 0;
    for (int row = 0; row < N; row++)
    {
        if (row == skip_row) continue;

        int dest_col = 0;
        for (int col = 0; col < N; col++)
        {
            if (col == skip_col) continue;

            result(dest_row, dest_col) = m(row, col);
            dest_col++;
        }
        dest_row++;
    }
    return result;
}

template <int N> requires (N >= 3)
float minor(const matrix<N>& m, int skip_row, int skip_col)
{
    return determinantMatrix(submatrix(m, skip_row, skip_col));
}

template <int N> requires (N >= 3)
float cofactor(const matrix<N>& m, int skip_row, int skip_col)
{
    float calculated_minor = minor(m, skip_row, skip_col);

    if ((skip_row + skip_col) % 2 != 0) {
        return -calculated_minor;
    }
    
    return calculated_minor;
}

#endif