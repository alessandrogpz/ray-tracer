#include <gtest/gtest.h>

import std;

import rt.materials;
import rt.colors;

using namespace rt;


// ---------------------------------------------------
// Material Creation

TEST(MaterialCreation, GenerateMaterial)
{
    const Material m;

    EXPECT_EQ(m.color, Color());
    EXPECT_EQ(m.ambient, 0.1f);
    EXPECT_EQ(m.diffuse, 0.9f);
    EXPECT_EQ(m.specular, 0.9f);
    EXPECT_EQ(m.shininess, 200.0f);
    EXPECT_FALSE(m.pattern.has_value());
}

TEST(MaterialCreation, ReflectivityForTheDefaultMaterial)
{
    const Material m;

    EXPECT_EQ(m.reflective, 0.0f);
}

TEST(MaterialCreation, TransparencyAndRefractiveIndexForTheDefaultMaterial)
{
    const Material m;

    EXPECT_EQ(m.transparency, 0.0f);
    EXPECT_EQ(m.refractive_index, 1.0f);
}

TEST(MaterialCreation, MaterialsCastShadowsByDefault)
{
    const Material m;

    EXPECT_TRUE(m.casts_shadow);
}
