#include <gtest/gtest.h>

import std;

import rt.tuple;
import rt.world;
import rt.sphere;
import rt.lights;
import rt.colors;
import rt.transformations;
import rt.ray;
import rt.intersection;

using namespace rt;

// ---------------------------------------------------
// World Creation

TEST(WorldCreation, EmptyWorld)
{
    const World w1;

    EXPECT_EQ(w1.spheres.empty(), true);
    EXPECT_EQ(w1.light, PointLight());
}

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

// ---------------------------------------------------
// Intersecting World

TEST(IntersectingWorld, IntersectWorldWithRay)
{
    const World w = default_world();
    const Ray r(Point(0.0, 0.0, -5.0), Vector(0.0, 0.0, 1.0));

    const std::vector<Intersection>xs = intersect_world(w, r);

    EXPECT_EQ(xs.size(), 4);
    EXPECT_EQ(xs[0].t, 4.0);
    EXPECT_EQ(xs[1].t, 4.5);
    EXPECT_EQ(xs[2].t, 5.5);
    EXPECT_EQ(xs[3].t, 6.0);
}

// ---------------------------------------------------
// Precomputing the State of an Intersection

TEST(PrecomputingInteraction, PrecomputeStateOfIntersection)
{
    const Ray r(Point(0.0, 0.0, -5.0), Vector(0.0, 0.0, 1.0));
    const Sphere s;
    World w;
    w.spheres.push_back(s);
    const Intersection i(4.0, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_DOUBLE_EQ(comps.intersection.t, i.t);
    EXPECT_EQ(comps.intersection.shape_index, i.shape_index);
    EXPECT_EQ(comps.intersection.shape_type, i.shape_type);

    EXPECT_EQ(comps.point, Point(0.0, 0.0, -1.0));
    EXPECT_EQ(comps.eye_v, Vector(0.0, 0.0, -1.0));
    EXPECT_EQ(comps.normal_v, Vector(0.0, 0.0, -1.0));

}

