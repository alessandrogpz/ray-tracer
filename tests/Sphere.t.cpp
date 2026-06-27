#include <gtest/gtest.h>

import std;

import rt.utils;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.sphere;
import rt.intersection;
import rt.materials;

using namespace rt;

// --------------------------------------------------
// Sphere Creation

TEST(SphereCreation, DefaultSphere)
{
    const auto s = Sphere();

    EXPECT_EQ(s.get_transform(), identity());
    EXPECT_EQ(s.origin, Point(0.0f, 0.0f, 0.0f));
    EXPECT_FLOAT_EQ(s.radius, 1.0f);
}

TEST(SphereCreation, UserCreatedSphere)
{
    auto s = Sphere(Point(1.0f, 2.0f, 3.0f), 2.5f);
    s.set_transform(translation(3.0f, 4.0f, 5.0f));

    EXPECT_EQ(s.origin, Point(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(s.get_transform(), translation(3.0f, 4.0f, 5.0f));
    EXPECT_FLOAT_EQ(s.radius, 2.5f);
}

// --------------------------------------------------
// Ray Intersection With Object

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTwoPoints)
{
    Point origin = createPoint(0.0f, 0.0f, -5.0f);
    Vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 4.0f);
    EXPECT_EQ(xs[1].t, 6.0f);
}

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTangent)
{
    Point origin = createPoint(0.0f, 0.0f, -5.0f);
    Vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = Ray(origin, direction);

    Point sphere_origin = createPoint(0.0f, 1.0f, 0.0f);
    auto s = Sphere(sphere_origin, 1.0f);

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 5.0f);
    EXPECT_EQ(xs[1].t, 5.0f);
}

TEST(SphereRayIntersection, RayMissesIntersectingWithSphere)
{
    Point origin = createPoint(0.0f, 0.0f, -5.0f);
    Vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = Ray(origin, direction);

    Point sphere_origin = createPoint(0.0f, 2.0f, 0.0f);
    auto s = Sphere(sphere_origin, 1.0f);

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

TEST(SphereRayIntersection, RayOriginatesInsideTheSphere)
{
    Point origin = createPoint(0.0f, 0.0f, 0.0f);
    Vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -1.0f);
    EXPECT_EQ(xs[1].t, 1.0f);
}

TEST(SphereRayIntersection, RayOriginatesInFrontOfTheSphere)
{
    Point origin = createPoint(0.0f, 0.0f, 5.0f);
    Vector direction = createVector(0.0f, 0.0f, 1.0f);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -6.0f);
    EXPECT_EQ(xs[1].t, -4.0f);
}

TEST(SphereRayIntersection, IntersectSetsTheObjectOnTheIntersection)
{
    auto r = Ray(createPoint(0.0f, 0.0f, -5.0f), createVector(0.0f, 0.0f, 1.0f));
    const auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    ASSERT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].shape_index, 0);
    EXPECT_EQ(xs[0].shape_type, ShapeType::Sphere);
    EXPECT_EQ(xs[1].shape_index, 0);
    EXPECT_EQ(xs[1].shape_type, ShapeType::Sphere);
}

// ---------------------------------------------------
// Sphere Transformation

TEST(SphereTransformation, ChangingSphereTranfrom)
{
    auto s = Sphere();
    const auto t = translation(2.0f, 3.0f, 4.0f);

    s.set_transform(t);

    EXPECT_EQ(s.get_transform(), t);
}

// ---------------------------------------------------
// Sphere Transformation + Ray Intercept

TEST(SphereTransformation, IntersectingAScaledSphereWithARay)
{
    auto r = Ray(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    auto s = Sphere();

    const auto t = scale(2.0f, 2.0f, 2.0f);
    s.set_transform(t);

    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 3);
    EXPECT_EQ(xs[1].t, 7);
}

TEST(SphereTransformation, IntersectingATranslatedSphereWithARay)
{
    auto r = Ray(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    auto s = Sphere();

    const auto t = translation(5.0f, 0.0f, 0.0f);
    s.set_transform(t);

    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

// ---------------------------------------------------
// Sphere Normal

TEST(SphereNormal, SphereNormalAtPointOnAxisX)
{
    const auto s = Sphere();
    const auto n = normalAt(s, Point(1, 0, 0));
    EXPECT_EQ(Vector(1, 0, 0), n);
}

TEST(SphereNormal, SphereNormalAtPointOnAxisy)
{
    const auto s = Sphere();
    const auto n = normalAt(s, Point(0, 1, 0));
    EXPECT_EQ(Vector(0, 1, 0), n);
}

TEST(SphereNormal, SphereNormalAtPointOnAxisz)
{
    const auto s = Sphere();
    const auto n = normalAt(s, Point(0, 0, 1));
    EXPECT_EQ(Vector(0, 0, 1), n);
}

TEST(SphereNormal, SphereNormalAtNonaxialPoint)
{
    const auto s = Sphere();
    const auto n = normalAt(s, Point(std::sqrt(3) / 3,
                                             std::sqrt(3) / 3,
                                             std::sqrt(3) / 3));

    EXPECT_EQ(Vector(std::sqrt(3) / 3,
                     std::sqrt(3) / 3,
                     std::sqrt(3) / 3), n);
}

TEST(SphereNormal, SphereNormalIsNormalized)
{
    const auto s = Sphere();
    const auto n = normalAt(s, Point(std::sqrt(3) / 3,
                                             std::sqrt(3) / 3,
                                             std::sqrt(3) / 3));

    EXPECT_EQ(normalizeVector(n), n);
}

TEST(SphereNormal, SphereNormalOnTranslatedSphere)
{
    auto s = Sphere();
    s.set_transform(translation(0, 1, 0));

    const auto n = normalAt(s, Point(0, 1.70711f, -0.70711f));
    EXPECT_EQ(Vector(0, 0.70711f, -0.70711f), n);
}

TEST(SphereNormal, SphereNormalOnATransformedSphere)
{
    auto s = Sphere();
    const auto m = scale(1, 0.5f, 1) * rotation_z(std::numbers::pi / 5.0f);
    s.set_transform(m);

    const auto n = normalAt(s, Point(0, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f));
    EXPECT_EQ(Vector(0, 0.97014f, -0.24254f), n);
}

// ---------------------------------------------------
// Sphere Material

TEST(SphereMaterial, SphereDefaultMaterial)
{
    const auto s = Sphere();

    EXPECT_EQ(s.material, Material());
}

TEST(SphereMaterial, SphereAssignedMaterial)
{
    auto s = Sphere();
    auto m = Material();
    m.ambient = 1.0f;
    s.material = m;

    EXPECT_EQ(s.material, m);
}
