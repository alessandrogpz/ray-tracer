#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "../includes/Utils.hpp"

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
};

#endif