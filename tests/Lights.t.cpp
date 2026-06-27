#include <gtest/gtest.h>

import rt.tuple;
import rt.colors;
import rt.lights;

using namespace rt;

// ---------------------------------------------------
// Light Point Creation

TEST(PointLightCreation, GeneratePointLight)
{
    PointLight pl;
    const Color white{1.0f, 1.0f, 1.0f};
    const Point origin{0.0f, 0.0f, 0.0f};

    pl.intensity = white;
    pl.position = origin;

    EXPECT_EQ(pl.intensity, white);
    EXPECT_EQ(pl.position, origin);
}
