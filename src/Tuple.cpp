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

tuple multiplyTupleByScalar(tuple a, float s)
{
	return tuple(a.x * s, a.y * s, a.z * s, a.w * s);
}

tuple divideTupleByScalar(tuple a, float s)
{
	if (s == 0)
		return tuple(0, 0, 0, 0);
	float r_s = 1.0f / s;
	return tuple(a.x * r_s, a.y * r_s, a.z * r_s, a.w * r_s);
}
