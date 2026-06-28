module;
#include <cmath>
module rt.utils;

import std;

namespace rt {
    bool equal(const float a, const float b)
    {
        return std::abs(a - b) < EPSILON;
    }
} // namespace rt
