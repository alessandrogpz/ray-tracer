#include <gtest/gtest.h>

import std;

import rt.utils;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.plane;
import rt.intersection;
import rt.materials;
import rt.colors;

using namespace rt;

constexpr float PI = std::numbers::pi_v<float>;

// --------------------------------------------------
// Plane Creation

TEST(PlaneCreation, DefaultPlane)
{
    const auto p = Plane();

    EXPECT_EQ(p.get_transform(), identity());
    EXPECT_EQ(p.material, Material());
}

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

TEST(PlaneNormal, PlaneNormalOnTranslatedPlane)
{
    Plane p;
    p.set_transform(translation(0.0f, 5.0f, 0.0f));
    
    // Normal of a translated horizontal plane remains pointing straight up
    const Vector n = normalAt(p, Point(0.0f, 5.0f, 0.0f));
    EXPECT_EQ(n, Vector(0.0f, 1.0f, 0.0f));
}

TEST(PlaneNormal, PlaneNormalOnATransformedPlane)
{
    Plane p;
    // Rotate the plane 90 degrees around X axis (makes the plane stand vertically facing along Z)
    p.set_transform(rotation_x(PI / 2.0f));
    
    // Normal should point along the Z axis (0, 0, 1) in world space
    const Vector n = normalAt(p, Point(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(n, Vector(0.0f, 0.0f, 1.0f));
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

TEST(PlaneIntersection, IntersectSetsTheObjectOnTheIntersection)
{
    const Plane p;
    const Ray r(Point(0.0f, 1.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));
    const std::vector<Intersection> xs = intersect(p, r, 5);

    ASSERT_EQ(xs.size(), 1);
    EXPECT_EQ(xs[0].shape_index, 5);
    EXPECT_EQ(xs[0].shape_type, ShapeType::Plane);
}

// --------------------------------------------------
// Plane Transformation Tests

TEST(PlaneTransformation, ChangingPlaneTransform)
{
    auto p = Plane();
    const auto t = translation(2.0f, 3.0f, 4.0f);
    p.set_transform(t);

    EXPECT_EQ(p.get_transform(), t);
}

TEST(PlaneTransformation, IntersectingATranslatedPlaneWithARay)
{
    auto p = Plane();
    p.set_transform(translation(0.0f, 2.0f, 0.0f));
    
    // Ray starts at y=5 and points straight down
    const Ray r(Point(0.0f, 5.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    ASSERT_EQ(xs.size(), 1);
    EXPECT_TRUE(equal(xs[0].t, 3.0f));
}

TEST(PlaneTransformation, IntersectingAScaledPlaneWithARay)
{
    auto p = Plane();
    p.set_transform(scale(2.0f, 2.0f, 2.0f));
    
    // Ray starts at y=5 and points straight down
    const Ray r(Point(0.0f, 5.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f));
    const std::vector<Intersection> xs = intersect(p, r);

    ASSERT_EQ(xs.size(), 1);
    EXPECT_TRUE(equal(xs[0].t, 5.0f));
}

// --------------------------------------------------
// Plane Material Tests

TEST(PlaneMaterial, PlaneDefaultMaterial)
{
    const auto p = Plane();
    EXPECT_EQ(p.material, Material());
}

TEST(PlaneMaterial, PlaneAssignedMaterial)
{
    auto p = Plane();
    Material m;
    m.color = Color(0.8f, 0.8f, 0.8f);
    p.material = m;

    EXPECT_EQ(p.material, m);
}
