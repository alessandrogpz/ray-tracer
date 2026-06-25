#include <gtest/gtest.h>

import rt.materials;
import rt.colors;

using namespace rt;


// ---------------------------------------------------
// Material Creation

TEST(MaterialCreation, GenerateMaterial)
{
    const Material m;

    EXPECT_EQ(m.color, Color());
    EXPECT_EQ(m.ambient, 0.1);
    EXPECT_EQ(m.diffuse, 0.9);
    EXPECT_EQ(m.specular, 0.9);
    EXPECT_EQ(m.shininess, 200.0);

}