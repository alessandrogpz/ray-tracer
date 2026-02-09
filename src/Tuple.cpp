#include <iostream>
#include "Tuple.h"

tuple createPoint(float x, float y, float z)
{
    return (tuple){x, y, z, 1.0f};
}

tuple createVector(float x, float y, float z)
{
    return (tuple){x, y, z, 0.0f};
}