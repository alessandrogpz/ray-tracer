#include "../includes/Tuple.hpp"
#include "../includes/Utils.hpp"
#include <cmath>


tuple createPoint(float x, float y, float z)
{
	return tuple(x, y, z, 1.0f);
}

tuple createVector(float x, float y, float z)
{
	return tuple(x, y, z, 0.0f);
}

tuple negateTuple(tuple a)
{
	return tuple(a * -1);
}

float getVectorMagnitude(tuple a)
{
	assert(equal(a.w, 0.0f) && "Argument 'a' to getVectorMagnitude must be a vector");

	// Uses the Pythagorean theorem: sqrt(x^2 + y^2 + z^2)
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

// Returns a vector with same direction but makes the length exactly 1.0
tuple normalizeVector(tuple a)
{
	assert(equal(a.w, 0.0f) && "Argument 'a' to normalizeVector must be a vector");

	// Normalizing a vector :
	// Devide each of its components by the vector magnitude
	float inv_mag = 1.0f / getVectorMagnitude(a);
	return createVector(a.x * inv_mag, a.y * inv_mag, a.z * inv_mag);
}

// Returns a scalar representing the "overlap" or angle between two vectors
float dotProduct(tuple a, tuple b)
{
	// ATTENTION: Only call dot product on normalized vectors.
	// This function does not have any asserts nor additional checks because it will be run billions of times.
	// Any additional comparison or assert adds delay to the computation.
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

// Returns a NEW vector perpendicular to both 'a' and 'b'
// Note: This only applies to geometric vectors (w=0)
// Note: The order of the arguments matter: (a, b) != (b, a).
tuple crossProduct(tuple a, tuple b)
{
	assert(equal(a.w, 0.0f) && "Argument 'a' to crossProduct must be a vector");
	assert(equal(b.w, 0.0f) && "Argument 'b' to crossProduct must be a vector");

	return (
		createVector(a.y * b.z - a.z * b.y,
					 a.z * b.x - a.x * b.z,
					 a.x * b.y - a.y * b.x));
}
