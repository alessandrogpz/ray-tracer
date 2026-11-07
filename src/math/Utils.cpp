module rt.utils;

import std;

namespace rt {
    bool equal(const float a, const float b)
    {
        // Identical values (including the same infinity, where a - b would be NaN)
        if (a == b)
            return true;

        // An infinity only equals the same infinity (handled above); without this check the
        // relative test below would accept it, since inf <= RELATIVE_EPSILON * inf
        if (std::isinf(a) || std::isinf(b))
            return false;

        const float difference = std::abs(a - b);

        // Absolute: near zero, where a relative tolerance would shrink to nothing
        if (difference < EPSILON)
            return true;

        // Relative: for large values, allow error proportional to their magnitude
        return difference <= RELATIVE_EPSILON * std::max(std::abs(a), std::abs(b));
    }
} // namespace rt
