#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;
import rt.ray;

using namespace rt;

// ---------------------------------------------------
// Creating a Ray

TEST(RayCreation, CreatingAndQueryingARay)
{
    point origin = createPoint(1.0f, 2.0f, 3.0f);
    vector direction = createVector(4.0f, 5.0f, 6.0f);

    ray r = ray(origin, direction);

    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}