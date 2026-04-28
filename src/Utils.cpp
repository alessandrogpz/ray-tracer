#include "Utils.hpp"

#include <cmath>

namespace rt {

	bool equal(float a, float b)
	{
		float EPSILON = 0.00001f;
		return std::abs(a - b) < EPSILON;
	}

} // namespace rt
