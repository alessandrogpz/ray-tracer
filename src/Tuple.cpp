#include <iostream>
#include <cmath>
#include "Tuple.h"

tuple createPoint(float x, float y, float z)
{
    return (tuple){x, y, z, 1.0f};
}

tuple createVector(float x, float y, float z)
{
    return (tuple){x, y, z, 0.0f};
}

bool equal(float a, float b)
{
    float EPSILON = 0.00001;

    if (std::abs(a - b) > EPSILON)
        return true;
    return false;
}
