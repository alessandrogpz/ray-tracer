#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <cmath>
#include <cassert>

struct tuple
{
    float x, y, z, w;

    tuple() : x(0), y(0), z(0), w(0) {}
    tuple(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}
};

tuple createPoint(float x, float y, float z);
tuple createVector(float x, float y, float z);

bool equal(float a, float b);

tuple addTuples(tuple a, tuple b);
tuple subtractTuples(tuple a, tuple b);
tuple negateTuple(tuple a);
tuple multiplyTupleByScalar(tuple a, float s);
tuple divideTupleByScalar(tuple a, float s);
float getVectorMagnitude(tuple a);
tuple normalizeVector(tuple a);
float dotProduct(tuple a, tuple b);
tuple crossProduct(tuple a, tuple b);

#endif