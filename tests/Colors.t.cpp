#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

// ------------------------------------------------------
// Color Creation

TEST(ColorCreation, CreateColorTuple)
{
	Color c = createColor(-0.5f, 0.4f, 1.7f);

	EXPECT_FLOAT_EQ(c.r, -0.5f);
	EXPECT_FLOAT_EQ(c.g, 0.4f);
	EXPECT_FLOAT_EQ(c.b, 1.7f);
}

// ------------------------------------------------------
// Color Operations

TEST(ColorOperation, AddTwoColors)
{
	Color c1 = createColor(0.9f, 0.6f, 0.75f);
	Color c2 = createColor(0.7f, 0.1f, 0.25f);

	// Operator Overload
	Color c_result = c1 + c2;

	EXPECT_FLOAT_EQ(c_result.r, 1.6f);
	EXPECT_FLOAT_EQ(c_result.g, 0.7f);
	EXPECT_FLOAT_EQ(c_result.b, 1.0f);
}

TEST(ColorOperation, SubtractTwoColors)
{
	Color c1 = createColor(0.9f, 0.6f, 0.75f);
	Color c2 = createColor(0.7f, 0.1f, 0.25f);

	// Operator Overload
	Color c_result = c1 - c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.2f);
	EXPECT_FLOAT_EQ(c_result.g, 0.5f);
	EXPECT_FLOAT_EQ(c_result.b, 0.5f);
}

TEST(ColorOperation, MultiplicationByScalar)
{
	Color c1 = createColor(0.2f, 0.3f, 0.4f);

	// Operator Overload
	Color c_result = c1 * 2;

	EXPECT_FLOAT_EQ(c_result.r, 0.4f);
	EXPECT_FLOAT_EQ(c_result.g, 0.6f);
	EXPECT_FLOAT_EQ(c_result.b, 0.8f);
}

TEST(ColorOperation, MultiplyTwoColors)
{
	Color c1 = createColor(1.0f, 0.2f, 0.4f);
	Color c2 = createColor(0.9f, 1.0f, 0.1f);

	// Operator Overload
	Color c_result = c1 * c2;

	EXPECT_FLOAT_EQ(c_result.r, 0.9f);
	EXPECT_FLOAT_EQ(c_result.g, 0.2f);
	EXPECT_FLOAT_EQ(c_result.b, 0.04f);
}