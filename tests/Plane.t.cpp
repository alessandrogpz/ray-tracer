#include <gtest/gtest.h>

import std;

import rt.utils;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.plane;
import rt.intersection;

using namespace rt;

// --------------------------------------------------
// Plane Normal Tests

TEST(PlaneNormal, NormalOfAPlaneIsConstantEverywhere)
{
    const Plane p;
    const Vector n1 = normalAt(p, Point(0.0f, 0.0f, 0.0f));
    const Vector n2 = normalAt(p, Point(10.0f, 0.0f, -10.0f));
    const Vector n3 = normalAt(p, Point(-5.0f, 0.0f, 150.0f));

    const Vector expected = Vector(0.0f, 1.0f, 0.0f);
    EXPECT_EQ(n1, expected);
    EXPECT_EQ(n2, expected);
    EXPECT_EQ(n3, expected);
}

// --------------------------------------------------
// Plane Intersection Tests

TEST(PlaneIntersection, IntersectWithARayParallelToThePlane)
{
    const Plane p;
    const Ray r(Point(0.0f, 10.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    EXPECT_TRUE(xs.empty());
}

TEST(PlaneIntersection, IntersectWithACoplanarRay)
{
    const Plane p;
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    EXPECT_TRUE(xs.empty());
}

TEST(PlaneIntersection, ARayIntersectingAPlaneFromAbove)
{
    const Plane p;
    const Ray r(Point(0.0f, 1.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    ASSERT_EQ(xs.size(), 1);
    EXPECT_TRUE(equal(xs[0].t, 1.0f));
    EXPECT_EQ(xs[0].shape_type, ShapeType::Plane);
}

TEST(PlaneIntersection, ARayIntersectingAPlaneFromBelow)
{
    const Plane p;
    const Ray r(Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    ASSERT_EQ(xs.size(), 1);
    EXPECT_TRUE(equal(xs[0].t, 1.0f));
    EXPECT_EQ(xs[0].shape_type, ShapeType::Plane);
}
