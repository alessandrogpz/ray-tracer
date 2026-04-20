#include "../includes/Transformations.hpp"

#include <cmath>

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
}

matrix<4> scale(float x, float y, float z) {
    
    matrix<4> scale = identity();

    scale(0, 0) = x;
    scale(1, 1) = y;
    scale(2, 2) = z;

    // | x    | 0.0  | 0.0  | 0.0  |
    // | 0.0  | y    | 0.0  | 0.0  |
    // | 0.0  | 0.0  | z    | 0.0  |
    // | 0.0  | 0.0  | 0.0  | 1.0  |

    return scale;
}

matrix<4> rotation_x(float radians) {
    
    matrix<4> transform = identity();
    
    transform(1, 1) = std::cos(radians);
    transform(1, 2) = -std::sin(radians);
    transform(2, 1) = std::sin(radians);
    transform(2, 2) = std::cos(radians);

    // | 1.0 | 0.0    |  0.0    | 0.0 |
    // | 0.0 | cos(r) | -sin(r) | 0.0 |
    // | 0.0 | sin(r) |  cos(r) | 0.0 |
    // | 0.0 | 0.0    |  0.0    | 1.0 |

    return transform;
}

matrix<4> rotation_y(float radians) {
    
    matrix<4> transform = identity();
    
    transform(0, 0) = std::cos(radians);
    transform(0, 2) = std::sin(radians);
    transform(2, 0) = -std::sin(radians);
    transform(2, 2) = std::cos(radians);

    // |  cos(r) | 0.0 | sin(r) | 0.0 |
    // |  0.0    | 1.0 | 0.0    | 0.0 |
    // | -sin(r) | 0.0 | cos(r) | 0.0 |
    // |  0.0    | 0.0 | 0.0    | 1.0 |

    return transform;
}

matrix<4> rotation_z(float radians) {
    
    matrix<4> transform = identity();
    
    transform(0, 0) = std::cos(radians);
    transform(0, 1) = -std::sin(radians);
    transform(1, 0) = std::sin(radians);
    transform(1, 1) = std::cos(radians);

    // | cos(r) | -sin(r) | 0.0 | 0.0 |
    // | sin(r) |  cos(r) | 0.0 | 0.0 |
    // | 0.0    |  0.0    | 1.0 | 0.0 |
    // | 0.0    |  0.0    | 0.0 | 1.0 |

    return transform;
}

matrix<4> shear(float xy, float xz, float yx, float yz, float zx, float zy) {

    matrix<4> transform = identity();

    transform(0, 1) = xy;
    transform(0, 2) = xz;
    transform(1, 0) = yx;
    transform(1, 2) = yz;
    transform(2, 0) = zx;
    transform(2, 1) = zy;

    // | 1.0  | xy   | xz   | 0.0  |
    // | yx   | 1.0  | yz   | 0.0  |
    // | zx   | zy   | 1.0  | 0.0  |
    // | 0.0  | 0.0  | 0.0  | 1.0  |

    return transform;
}