export module rt.matrix;

import std;

import rt.utils;
import rt.tuple;

export namespace rt {

    template <std::size_t N>
    struct Matrix {
        float data[N][N]{};

        // Default constructor: Initializes all elements to 0
        Matrix() {
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    data[i][j] = 0.0f;
                }
            }
        }

        // Getters & Setters -----------------------------------------------------------------
        // Setter: m(row, col)
        float& operator()(std::size_t row, std::size_t col) {
            return data[row][col];
        }

        // Getter
        float operator()(std::size_t row, std::size_t col) const {
            return data[row][col];
        }

        // Overload Operators ----------------------------------------------------------------
        // Matrix Comparison (==)
        [[nodiscard]]
        friend inline bool operator==(const Matrix<N>& a, const Matrix<N>& b) {
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    if (!equal(a(r, c), b(r, c))) {
                        return false;
                    }
                }
            }
            return true;
        }

        // Matrix Comparison (!=)
        [[nodiscard]]
        friend inline bool operator!=(const Matrix<N>& a, const Matrix<N>& b) {
            return !(a == b);
        }

        // Matrix * Scalar
        [[nodiscard]]
        friend inline Matrix<N> operator*(const Matrix<N>& a, const float s)
        {
            Matrix<N> result{};
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    result(r, c) = a(r, c) * s;
                }
            }
            return result;
        }

        // Matrix + Matrix
        [[nodiscard]]
        friend inline Matrix<N> operator+(const Matrix<N>&a, const Matrix<N>& b)
        {
            Matrix<N> result{};
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    result(r, c) = a(r, c) + b(r, c);
                }
            }
            return result;
        }

        // Matrix - Matrix
        [[nodiscard]]
        friend inline Matrix<N> operator-(const Matrix<N>&a, const Matrix<N>& b)
        {
            Matrix<N> result{};
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    result(r, c) = a(r, c) - b(r, c);
                }
            }
            return result;
        }

        // Matrix * Matrix
        [[nodiscard]]
        friend inline Matrix<N> operator*(const Matrix<N>& a, const Matrix<N>& b) {
            Matrix<N> result;
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    float sum = 0.0f;
                    for (std::size_t k = 0; k < N; k++) {
                        sum += a(r, k) * b(k, c);
                    }
                    result(r, c) = sum;
                }
            }
            return result;
        }

        // Matrix * Tuple (Base)
        [[nodiscard]]
        friend inline Tuple operator*(const Matrix<N>& m, const Tuple& t) requires (N == 4) {
            float res[4] = {0, 0, 0, 0};
            for (std::size_t r = 0; r < 4; r++) {
                for (std::size_t c = 0; c < 4; c++) {
                    res[r] += m(r, c) * t[c];
                }
            }
            return {res[0], res[1], res[2], res[3]};
        }

        // Matrix * Point
        [[nodiscard]]
        friend inline Point operator*(const Matrix<N>& m, const Point& p) requires (N == 4) {
            return Point(m * static_cast<const Tuple&>(p));
        }

        // Matrix * Vector
        [[nodiscard]]
        friend inline Vector operator*(const Matrix<N>& m, const Vector& v) requires (N == 4) {
            return Vector(m * static_cast<const Tuple&>(v));
        }

        // Matrix Operations --------------------------------------------------------------------
        [[nodiscard]]
        Matrix<N> transpose() const {
            Matrix<N> result;
            for (std::size_t r = 0; r < N; r++) {
                for (std::size_t c = 0; c < N; c++) {
                    result(c, r) = (*this)(r, c);
                }
            }
            return result;
        }

        [[nodiscard]]
        float determinant() const {
            if constexpr (N == 2) {
                return ((*this)(0,0) * (*this)(1,1)) - ((*this)(0,1) * (*this)(1,0));
            } else {
                float det = 0.0f;
                for (std::size_t col = 0; col < N; ++col) {
                    det += (*this)(0, col) * cofactor(0, col);
                }
                return det;
            }
        }

        // Returns a smaller (N-1) Matrix by dropping the specified row and column.
        [[nodiscard]]
        Matrix<N-1> submatrix(std::size_t skip_row, std::size_t skip_col) const requires (N >= 2) {
            Matrix<N-1> result;
            std::size_t dest_row = 0;
            for (std::size_t row = 0; row < N; row++) {
                if (row == skip_row) continue;
                std::size_t dest_col = 0;
                for (std::size_t col = 0; col < N; col++) {
                    if (col == skip_col) continue;
                    result(dest_row, dest_col) = (*this)(row, col);
                    dest_col++;
                }
                dest_row++;
            }
            return result;
        }

        [[nodiscard]]
        float minor(std::size_t skip_row, std::size_t skip_col) const requires (N >= 3) {
            return submatrix(skip_row, skip_col).determinant();
        }

        [[nodiscard]]
        float cofactor(std::size_t skip_row, std::size_t skip_col) const requires (N >= 3) {
            float calc_minor = minor(skip_row, skip_col);
            if ((skip_row + skip_col) % 2 != 0) {
                return -calc_minor;
            }
            return calc_minor;
        }

        [[nodiscard]]
        bool isInvertible() const {
            return !equal(determinant(), 0.0f);
        }

        // Throws std::runtime_error if the Matrix cannot be inverted (determinant is 0).
        [[nodiscard]]
        Matrix<N> inverse() const
        {
            if (!isInvertible()) {
                throw std::runtime_error("Attempted to invert a non-invertible Matrix.");
            }

            Matrix<N> result;
            float current_det = determinant();

            for (std::size_t row = 0; row < N; row++)
            {
                for (std::size_t col = 0; col < N; col++)
                {
                    float c = cofactor(col, row);
                    result(row, col) = c / current_det;
                }
            }

            return result;
        }
    };

} // namespace rt
