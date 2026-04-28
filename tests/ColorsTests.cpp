#include <gtest/gtest.h>

#include "Colors.hpp"

// ------------------------------------------------------
// Color Creation

TEST(ColorCreation, CreateColorTuple)
{
	color c = createColor(-0.5f, 0.4f, 1.7f);

	EXPECT_FLOAT_EQ(c.r, -0.5f);
	EXPECT_FLOAT_EQ(c.g, 0.4f);
	EXPECT_FLOAT_EQ(c.b, 1.7f);
}

// ------------------------------------------------------
// Color Operations

TEST(ColorOperation, AddTwoColors)
{
	color c1 = createColor(0.9f, 0.6f, 0.75f);
	color c2 = createColor(0.7f, 0.1f, 0.25f);

	// Operator Overload
	color c_result = c1 + c2;

	EXPECT_FLOAT_EQ(c_result.r, 1.6f);
	EXPECT_FLOAT_EQ(c_result.g, 0.7f);
	EXPECT_FLOAT_EQ(c_result.b, 1.0f);
}

TEST(ColorOperation, SubtractTwoColors)
{
	color c1 = createColor(0.9f, 0.6f, 0.75f);
	color c2 = createColor(0.7f, 0.1f, 0.25f);

	// Operator Overload
	color c_result = c1 - c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.2f);
	EXPECT_FLOAT_EQ(c_result.g, 0.5f);
	EXPECT_FLOAT_EQ(c_result.b, 0.5f);
}

TEST(ColorOperation, MultiplicationByScalar)
{
	color c1 = createColor(0.2f, 0.3f, 0.4f);

	// Operator Overload
	color c_result = c1 * 2;

	EXPECT_FLOAT_EQ(c_result.r, 0.4f);
	EXPECT_FLOAT_EQ(c_result.g, 0.6f);
	EXPECT_FLOAT_EQ(c_result.b, 0.8f);
}

TEST(ColorOperation, MultiplyTwoColors)
{
	color c1 = createColor(1.0f, 0.2f, 0.4f);
	color c2 = createColor(0.9f, 1.0f, 0.1f);

	// Operator Overload
	color c_result = c1 * c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.9f);
	EXPECT_FLOAT_EQ(c_result.g, 0.2f);
	EXPECT_FLOAT_EQ(c_result.b, 0.04f);
}