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

    EXPECT_EQ(w1.sphere_origins.empty(), true);
    EXPECT_EQ(w1.light, PointLight());
}

TEST(WorldCreation, DefaultWorld)
{
    PointLight expected_light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Sphere s1;
    s1.material.color = Color(0.8f, 1.0f, 0.6f);
    s1.material.diffuse = 0.7f;
    s1.material.specular = 0.2f;

    Sphere s2;
    s2.set_transform(scale(0.5f, 0.5f, 0.5f));

    World w = default_world();

    EXPECT_EQ(w.light, expected_light);

    ASSERT_EQ(w.sphere_origins.size(), 2);
    // Check s1 properties
    EXPECT_EQ(w.sphere_materials[0].color, Color(0.8f, 1.0f, 0.6f));
    EXPECT_EQ(w.sphere_materials[0].diffuse, 0.7f);
    EXPECT_EQ(w.sphere_materials[0].specular, 0.2f);

    // Check s2 properties
    EXPECT_EQ(w.sphere_transforms[1], scale(0.5f, 0.5f, 0.5f));
}

// ---------------------------------------------------
// Intersecting World

TEST(IntersectingWorld, IntersectWorldWithRay)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));

    const std::vector<Intersection>xs = intersect_world(w, r);

    EXPECT_EQ(xs.size(), 4);
    EXPECT_EQ(xs[0].t, 4.0f);
    EXPECT_EQ(xs[1].t, 4.5f);
    EXPECT_EQ(xs[2].t, 5.5f);
    EXPECT_EQ(xs[3].t, 6.0f);
}

// ---------------------------------------------------
// Precomputing the State of an Intersection

TEST(PrecomputingInteraction, PrecomputeStateOfIntersection)
{
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Sphere s;
    World w;
    w.add_sphere(s);
    const Intersection i(4.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_FLOAT_EQ(comps.intersection.t, i.t);
    EXPECT_EQ(comps.intersection.shape_index, i.shape_index);
    EXPECT_EQ(comps.intersection.shape_type, i.shape_type);

    EXPECT_EQ(comps.point, Point(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(comps.eye_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(comps.normal_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_FALSE(comps.inside);
}

TEST(PrecomputingInteraction, PrecomputeStateOfIntersectionInside)
{
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const Sphere s;
    World w;
    w.add_sphere(s);
    const Intersection i(1.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_FLOAT_EQ(comps.intersection.t, i.t);
    EXPECT_EQ(comps.point, Point(0.0f, 0.0f, 1.0f));
    EXPECT_EQ(comps.eye_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_TRUE(comps.inside);
    // The normal vector is inverted since it was pointing outwards
    EXPECT_EQ(comps.normal_v, Vector(0.0f, 0.0f, -1.0f));
}

// ---------------------------------------------------
// Shading an Intersection

TEST(ShadingIntersection, ShadingIntersectionFormOutside)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(4.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);
    const Color c = shade_hit(w, comps);

    EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f));
}

TEST(ShadingIntersection, ShadingIntersectionFormInside)
{
    World w = default_world();
    w.light = PointLight(Point(0.0f, 0.25f, 0.0f), Color(1.0f, 1.0f, 1.0f));
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(0.5f, 1, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);
    const Color c = shade_hit(w, comps);

    EXPECT_EQ(c, Color(0.90498f, 0.90498f, 0.90498f));
}

// ---------------------------------------------------
// Color At a Ray

TEST(ColorAtRay, ColorWhenRayMisses)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 1.0f, 0.0f));
    const Color c = color_at(w, r);

    EXPECT_EQ(c, Color(0.0f, 0.0f, 0.0f));
}

TEST(ColorAtRay, ColorWhenRayHits)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Color c = color_at(w, r);

    EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f));
}

TEST(ColorAtRay, ColorWithIntersectionBehindRay)
{
    World w = default_world();
    
    // Set ambient to 1 for both spheres so they glow completely in their own color
    w.sphere_materials[0].ambient = 1.0f;
    w.sphere_materials[1].ambient = 1.0f;

    // Ray originates inside the outer sphere but points at the inner sphere from its edge
    const Ray r(Point(0.0f, 0.0f, 0.75f), Vector(0.0f, 0.0f, -1.0f));
    const Color c = color_at(w, r);

    // Should return the color of the inner sphere
    EXPECT_EQ(c, w.sphere_materials[1].color);
}
