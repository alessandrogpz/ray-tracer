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

    matrix<N> transpose() const {
        matrix<N> result;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                result(c, r) = (*this)(r, c);
            }
        }
        return result;
    }

    float determinant() const {
        if constexpr (N == 2) {
            return ((*this)(0,0) * (*this)(1,1)) - ((*this)(0,1) * (*this)(1,0));
        } else {
            float det = 0.0f;
            for (int col = 0; col < N; ++col) {
                det += (*this)(0, col) * cofactor(0, col);
            }
            return det;
        }
    }

    matrix<N-1> submatrix(int skip_row, int skip_col) const requires (N >= 2) {   
        matrix<N-1> result;
        int dest_row = 0;
        for (int row = 0; row < N; row++) {
            if (row == skip_row) continue;
            int dest_col = 0;
            for (int col = 0; col < N; col++) {
                if (col == skip_col) continue;
                result(dest_row, dest_col) = (*this)(row, col);
                dest_col++;
            }
            dest_row++;
        }
        return result;
    }

    float minor(int skip_row, int skip_col) const requires (N >= 3) {
        return submatrix(skip_row, skip_col).determinant();
    }

    float cofactor(int skip_row, int skip_col) const requires (N >= 3) {
        float calc_minor = minor(skip_row, skip_col);
        if ((skip_row + skip_col) % 2 != 0) {
            return -calc_minor;
        }
        return calc_minor;
    }

    bool isInvertible() const {
        return !equal(determinant(), 0.0f);
    }
};

#endif