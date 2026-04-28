module;

#include <cstddef>
#include <stdexcept>

export module rt.matrix;

import rt.utils;
import rt.tuple;

export namespace rt {

    template <size_t N>
    struct matrix {
        float data[N][N];

        // Default constructor: Initializes all elements to 0
        matrix() {
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    data[i][j] = 0.0f;
                }
            }
        }

        // Setter: m(row, col)
        float& operator()(size_t row, size_t col) {
            return data[row][col];
        }

        // Getter
        float operator()(size_t row, size_t col) const {
            return data[row][col];
        }

        // Overload for matrix comparison (==)
        [[nodiscard]] friend inline bool operator==(const matrix<N>& a, const matrix<N>& b) {
            for (size_t r = 0; r < N; r++) {
                for (size_t c = 0; c < N; c++) {
                    if (!equal(a(r, c), b(r, c))) {
                        return false;
                    }
                }
            }
            return true;
        }

        // Overload for matrix comparison (!=)
        [[nodiscard]] friend inline bool operator!=(const matrix<N>& a, const matrix<N>& b) {
            return !(a == b);
        }

        // Overload for matrix multiplication
        [[nodiscard]] friend inline matrix<N> operator*(const matrix<N>& a, const matrix<N>& b){
            matrix<N> result;

            for (size_t r = 0; r < N; r++) {
                for (size_t c = 0; c < N; c++) {
                    float sum = 0.0f;
                    for (size_t k = 0; k < N; k++) {
                        sum += a(r, k) * b(k, c);
                    }
                    result(r, c) = sum;
                }
            }
            return result;
        }

        // Overload for matrix * tuple multiplication
        [[nodiscard]] friend inline tuple operator*(const matrix<N>& m, const tuple& t) requires (N == 4) {
            float res[4] = {0, 0, 0, 0};

            for (size_t r = 0; r < 4; r++) {
                for (size_t c = 0; c < 4; c++) {
                    res[r] += m(r, c) * t[c];
                }
            }

            return tuple(res[0], res[1], res[2], res[3]);
        }

        [[nodiscard]] matrix<N> transpose() const {
            matrix<N> result;
            for (size_t r = 0; r < N; r++) {
                for (size_t c = 0; c < N; c++) {
                    result(c, r) = (*this)(r, c);
                }
            }
            return result;
        }

        [[nodiscard]] float determinant() const {
            if constexpr (N == 2) {
                return ((*this)(0,0) * (*this)(1,1)) - ((*this)(0,1) * (*this)(1,0));
            } else {
                float det = 0.0f;
                for (size_t col = 0; col < N; ++col) {
                    det += (*this)(0, col) * cofactor(0, col);
                }
                return det;
            }
        }

        [[nodiscard]] matrix<N-1> submatrix(size_t skip_row, size_t skip_col) const requires (N >= 2) {   
            matrix<N-1> result;
            size_t dest_row = 0;
            for (size_t row = 0; row < N; row++) {
                if (row == skip_row) continue;
                size_t dest_col = 0;
                for (size_t col = 0; col < N; col++) {
                    if (col == skip_col) continue;
                    result(dest_row, dest_col) = (*this)(row, col);
                    dest_col++;
                }
                dest_row++;
            }
            return result;
        }

        [[nodiscard]] float minor(size_t skip_row, size_t skip_col) const requires (N >= 3) {
            return submatrix(skip_row, skip_col).determinant();
        }

        [[nodiscard]] float cofactor(size_t skip_row, size_t skip_col) const requires (N >= 3) {
            float calc_minor = minor(skip_row, skip_col);
            if ((skip_row + skip_col) % 2 != 0) {
                return -calc_minor;
            }
            return calc_minor;
        }

        [[nodiscard]] bool isInvertible() const {
            return !equal(determinant(), 0.0f);
        }

        [[nodiscard]] matrix<N> inverse() const
        {
            if (!isInvertible()) {
                throw std::runtime_error("Attempted to invert a non-invertible matrix.");
            }

            matrix<N> result;
            float current_det = determinant();

            for (size_t row = 0; row < N; row++)
            {
                for (size_t col = 0; col < N; col++)
                {
                    float c = cofactor(col, row); 
                    result(row, col) = c / current_det;
                }
            }
            
            return result;
        }
    };

} // namespace rt
