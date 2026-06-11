module;

#include <cmath>

export module rt.utils;

export namespace rt {

    bool equal(double a, double b)
    {
        constexpr double EPSILON = 0.00001;
        return std::abs(a - b) < EPSILON;
    }

} // namespace rt
