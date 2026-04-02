#include <gtest/gtest.h>
#include "../includes/Colors.hpp"

// ------------------------------------------------------
// Color Creation

TEST(ColorCreation, CreateColorTuple)
{
	color c = createColor(-0.5, 0.4, 1.7);

	EXPECT_FLOAT_EQ(c.r, -0.5);
	EXPECT_FLOAT_EQ(c.g, 0.4);
	EXPECT_FLOAT_EQ(c.b, 1.7);
}

// ------------------------------------------------------
// Color Operations

TEST(ColorOperation, AddTwoColors)
{
	color c1 = createColor(0.9, 0.6, 0.75);
	color c2 = createColor(0.7, 0.1, 0.25);

	// Operator Overload
	color c_result = c1 + c2;

	EXPECT_FLOAT_EQ(c_result.r, 1.6);
	EXPECT_FLOAT_EQ(c_result.g, 0.7);
	EXPECT_FLOAT_EQ(c_result.b, 1.0);
}

TEST(ColorOperation, SubtractTwoColors)
{
	color c1 = createColor(0.9, 0.6, 0.75);
	color c2 = createColor(0.7, 0.1, 0.25);

	// Operator Overload
	color c_result = c1 - c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.2);
	EXPECT_FLOAT_EQ(c_result.g, 0.5);
	EXPECT_FLOAT_EQ(c_result.b, 0.5);
}

TEST(ColorOperation, MultiplicationByScalar)
{
	color c1 = createColor(0.2, 0.3, 0.4);

	// Operator Overload
	color c_result = c1 * 2;

	EXPECT_FLOAT_EQ(c_result.r, 0.4);
	EXPECT_FLOAT_EQ(c_result.g, 0.6);
	EXPECT_FLOAT_EQ(c_result.b, 0.8);
}

TEST(ColorOperation, MultiplyTwoColors)
{
	color c1 = createColor(1, 0.2, 0.4);
	color c2 = createColor(0.9, 1, 0.1);

	// Operator Overload
	color c_result = c1 * c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.9);
	EXPECT_FLOAT_EQ(c_result.g, 0.2);
	EXPECT_FLOAT_EQ(c_result.b, 0.04);
}