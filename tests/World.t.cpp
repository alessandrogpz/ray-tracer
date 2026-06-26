#include <gtest/gtest.h>

import std;

import rt.tuple;
import rt.world;
import rt.sphere;
import rt.lights;
import rt.colors;
import rt.transformations;

using namespace rt;

// ---------------------------------------------------
// World Creation

TEST(WorldCreation, EmptyWorld)
{
    const World w1;

    EXPECT_EQ(w1.spheres.empty(), true);
    EXPECT_EQ(w1.light, PointLight());
}

// Default World Creation
TEST(WorldCreation, DefaultWorld)
{
    PointLight expected_light(Point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

    Sphere s1;
    s1.material.color = Color(0.8, 1.0, 0.6);
    s1.material.diffuse = 0.7;
    s1.material.specular = 0.2;

    Sphere s2;
    s2.set_transform(scale(0.5, 0.5, 0.5));

    World w = default_world();

    EXPECT_EQ(w.light, expected_light);

    ASSERT_EQ(w.spheres.size(), 2);
    // Check s1 properties
    EXPECT_EQ(w.spheres[0].material.color, Color(0.8, 1.0, 0.6));
    EXPECT_EQ(w.spheres[0].material.diffuse, 0.7);
    EXPECT_EQ(w.spheres[0].material.specular, 0.2);

    // Check s2 properties
    EXPECT_EQ(w.spheres[1].get_transform(), scale(0.5, 0.5, 0.5));
}
