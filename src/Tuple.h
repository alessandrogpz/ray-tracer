#pragma once

struct tuple
{
    float x, y, z, w;

    tuple(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}
};

tuple createPoint(float x, float y, float z);
tuple createVector(float x, float y, float z);

bool equal(float a, float b);

tuple addTuples(tuple a, tuple b);