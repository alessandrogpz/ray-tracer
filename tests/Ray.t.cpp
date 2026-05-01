#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.shapes;
import rt.intersection;

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

// ---------------------------------------------------
// Ray Operations

TEST(RayOperation, ComputingPointFromDistance)
{
    ray r = ray(point(2.0f, 3.0f, 4.0f), vector(1.0f, 0.0f, 0.0f));

    point pos1 = position(r, 0);
    point pos2 = position(r, 1);
    point pos3 = position(r, -1);
    point pos4 = position(r, 2.5);

    EXPECT_EQ(pos1, point(2.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos2, point(3.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos3, point(1.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos4, point(4.5f, 3.0f, 4.0f));
}

// --------------------------------------------------
// Ray Intersection With Object

TEST(RayIntersection, IntersectingRayWithSphereAtTwoPoints)
{
    point origin = createPoint(0.0f, 0.0f, -5.0f);
    vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 4.0f);
    EXPECT_EQ(xs[1].t, 6.0f);
}

TEST(RayIntersection, IntersectingRayWithSphereAtTangent)
{
    point origin = createPoint(0.0f, 0.0f, -5.0f);
    vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = ray(origin, direction);

    point sphere_origin = createPoint(0.0f, 1.0f, 0.0f);
    auto s = sphere(sphere_origin, 1.0f);

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 5.0f);
    EXPECT_EQ(xs[1].t, 5.0f);
}

TEST(RayIntersection, RayMissesIntersectingWithSphere)
{
    point origin = createPoint(0.0f, 0.0f, -5.0f);
    vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = ray(origin, direction);

    point sphere_origin = createPoint(0.0f, 2.0f, 0.0f);
    auto s = sphere(sphere_origin, 1.0f);

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

TEST(RayIntersection, RayOriginatesInsideTheSphere)
{
    point origin = createPoint(0.0f, 0.0f, 0.0f);
    vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -1.0f);
    EXPECT_EQ(xs[1].t, 1.0f);
}

TEST(RayIntersection, RayOriginatesInFrontOfTheSphere)
{
    point origin = createPoint(0.0f, 0.0f, 5.0f);
    vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -6.0f);
    EXPECT_EQ(xs[1].t, -4.0f);
}

TEST(SphereIntersectionTest, IntersectSetsTheObjectOnTheIntersection)
{
    auto r = ray(createPoint(0.0f, 0.0f, -5.0f), createVector(0.0f, 0.0f, 1.0f));
    const auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    ASSERT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].obj, &s);
    EXPECT_EQ(xs[1].obj, &s);
}

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHavePositiveT)
{
    const auto s = sphere();
    const auto i1 = intersection(1.0f, &s);
    const auto i2 = intersection(2.0f, &s);

    std::vector<intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i1.t);
    EXPECT_EQ(i->obj, i1.obj);
}

TEST(SphereIntersectionTests, HitWhenSomeIntersectionsHaveNegativeT)
{
    const auto s = sphere();
    const auto i1 = intersection(-1.0f, &s);
    const auto i2 = intersection(2.0f, &s);

    std::vector<intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i2.t);
    EXPECT_EQ(i->obj, i2.obj);
}

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHaveNegativeT)
{
    const auto s = sphere();
    const auto i1 = intersection(-2.0f, &s);
    const auto i2 = intersection(-1.0f, &s);

    std::vector<intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FALSE(i);
}

TEST(SphereIntersectionTests, HitTheLowestNonnegativeIntersection)
{
    const auto s = sphere();
    const auto i1 = intersection(5.0f, &s);
    const auto i2 = intersection(7.0f, &s);
    const auto i3 = intersection(-3.0f, &s);
    const auto i4 = intersection(2.0f, &s);

    std::vector<intersection> xs = {i1, i2, i3, i4};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i4.t);
    EXPECT_EQ(i->obj, i4.obj);
}
