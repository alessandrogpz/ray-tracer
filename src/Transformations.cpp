#include "../includes/Transformations.hpp"

matrix<4> identity() {
    matrix<4> I; // Default all components to 0.0f

    I(0, 0) = 1.0f;
    I(1, 1) = 1.0f;
    I(2, 2) = 1.0f;
    I(3, 3) = 1.0f;

    // | 1.0  | 0.0  | 0.0  | 0.0  |
    // | 0.0  | 1.0  | 0.0  | 0.0  |
    // | 0.0  | 0.0  | 1.0  | 0.0  |
    // | 0.0  | 0.0  | 0.0  | 1.0  |

    return I;
}

matrix<4> translation(float x, float y, float z) {

    matrix<4> transform = identity();

    transform(0, 3) = x;
    transform(1, 3) = y;
    transform(2, 3) = z;

    // | 1.0  | 0.0  | 0.0  | x    |
    // | 0.0  | 1.0  | 0.0  | y    |
    // | 0.0  | 0.0  | 1.0  | z    |
    // | 0.0  | 0.0  | 0.0  | 1.0  |

    return transform;
