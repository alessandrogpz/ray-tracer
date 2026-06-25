#include <gtest/gtest.h>

import rt.tuple;
import rt.colors;
import rt.lights;

using namespace rt;

// ---------------------------------------------------
// Light Point Creation

TEST(PointLightCreation, GeneratePointLight)
{
    point_light pl;
    const color white{1.0, 1.0, 1.0};
    const point origin{0.0, 0.0, 0.0};

    pl.intensity = white;
    pl.position = origin;

    EXPECT_EQ(pl.intensity, white);
    EXPECT_EQ(pl.position, origin);
}
