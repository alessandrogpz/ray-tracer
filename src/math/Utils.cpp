module;
#include <cmath>
module rt.utils;

import std;

namespace rt {
    bool equal(double a, double b)
    {
        constexpr double EPSILON = 0.00001;
        return std::abs(a - b) < EPSILON;
    }
} // namespace rt
