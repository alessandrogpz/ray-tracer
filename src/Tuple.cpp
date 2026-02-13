#include <iostream>
#include <cmath>
#include "Tuple.hpp"

tuple createPoint(float x, float y, float z)
{
	return tuple(x, y, z, 1.0f);
}

tuple createVector(float x, float y, float z)
{
	return tuple(x, y, z, 0.0f);
}

bool equal(float a, float b)
{
	float EPSILON = 0.00001;

	if (std::abs(a - b) > EPSILON)
		return true;
	return false;
}

tuple addTuples(tuple a, tuple b)
{
	return tuple(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

tuple subtractTuples(tuple a, tuple b)
{
	return tuple(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

tuple negateTuple(tuple a)
{
	return tuple(-a.x, -a.y, -a.z, -a.w);
}
