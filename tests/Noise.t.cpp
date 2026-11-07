#include <gtest/gtest.h>

import std;

import rt.noise;

using namespace rt;

// ---------------------------------------------------
// Perlin Noise

TEST(PerlinNoise, NoiseIsZeroAtIntegerLatticePoints)
{
    for (int x = -3; x <= 3; ++x)
        for (int y = -3; y <= 3; ++y)
            for (int z = -3; z <= 3; ++z)
                EXPECT_FLOAT_EQ(perlin_noise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)), 0.0f);
}

TEST(PerlinNoise, NoiseIsDeterministic)
{
    EXPECT_FLOAT_EQ(perlin_noise(1.3f, -2.7f, 0.45f), perlin_noise(1.3f, -2.7f, 0.45f));
    EXPECT_FLOAT_EQ(perlin_noise(10.5f, 3.25f, -7.75f), perlin_noise(10.5f, 3.25f, -7.75f));
}

TEST(PerlinNoise, NoiseStaysWithinMinusOneAndOne)
{
    for (int i = 0; i < 2000; ++i)
    {
        const float t = static_cast<float>(i) * 0.137f;
        const float n = perlin_noise(t, t * 0.71f - 3.0f, 5.0f - t * 1.3f);
        EXPECT_GE(n, -1.0f);
        EXPECT_LE(n, 1.0f);
    }
}

TEST(PerlinNoise, NoiseVariesBetweenLatticePoints)
{
    float smallest = 1.0f;
    float largest = -1.0f;
    for (int i = 0; i < 200; ++i)
    {
        const float t = static_cast<float>(i) * 0.31f + 0.17f;
        const float n = perlin_noise(t, t * 0.5f + 0.3f, t * 0.25f + 0.6f);
        smallest = std::min(smallest, n);
        largest = std::max(largest, n);
    }

    // Both signs appear and the spread is substantial, i.e. the noise is not flat
    EXPECT_LT(smallest, -0.1f);
    EXPECT_GT(largest, 0.1f);
}

TEST(PerlinNoise, NoiseIsContinuous)
{
    // A tiny step in the input only produces a tiny change in the output
    for (int i = 0; i < 500; ++i)
    {
        const float x = static_cast<float>(i) * 0.0523f - 10.0f;
        const float a = perlin_noise(x, 1.7f, -2.3f);
        const float b = perlin_noise(x + 0.001f, 1.7f, -2.3f);
        EXPECT_LT(std::abs(a - b), 0.01f) << "at x = " << x;
    }
}
