#include <gtest/gtest.h>

import rt.intersection;
import rt.shapes;

using namespace rt;

// ---------------------------------------------------------
// Creating a Intersection

TEST(IntersectionCreation, CreatingAndQueryingIntersection)
{
    auto s = sphere();
    auto i = intersection(3.5f, &s);

    EXPECT_FLOAT_EQ(i.t, 3.5f);
    EXPECT_EQ(i.obj, &s);
}

//-------------------------------------------------------------
// Aggregating multiple intersections into a standard vector.
TEST(IntersectionTest, AggregatingIntersections)
{
    const auto s = sphere();
    auto i1 = intersection(1.0f, &s);
    auto i2 = intersection(2.0f, &s);

    std::vector<intersection> xs = {i1, i2};

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 1.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 2.0f);
}