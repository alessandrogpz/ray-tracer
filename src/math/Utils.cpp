module;
#include <cmath>
module rt.utils;

import std;

namespace rt {
    bool equal(float a, float b)
    {
        constexpr float EPSILON = 0.0001f;
        return std::abs(a - b) < EPSILON;
    }
} // namespace rt
