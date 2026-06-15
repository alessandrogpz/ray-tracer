#include <complex>
#include <gtest/gtest.h>
#include <numbers>

import rt.utils;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.shapes;
import rt.intersection;

using namespace rt;

// --------------------------------------------------
// Sphere Creation

TEST(SphereCreation, DefaultSphere)
{
    const auto s = sphere();

    EXPECT_EQ(s.transform, identity());
    EXPECT_EQ(s.origin, point(0.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(s.radius, 1.0);
}

TEST(SphereCreation, UserCreatedSphere)
{
    auto s = sphere(point(1.0, 2.0, 3.0), 2.5);
    s.transform = translation(3.0, 4.0, 5.0);

    EXPECT_EQ(s.origin, point(1.0, 2.0, 3.0));
    EXPECT_EQ(s.transform, translation(3.0, 4.0, 5.0));
    EXPECT_DOUBLE_EQ(s.radius, 2.5);
}

// --------------------------------------------------
// Ray Intersection With Object

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTwoPoints)
{
    point origin = createPoint(0.0, 0.0, -5.0);
    vector direction = createVector(0.0, 0.0, 1.0);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 4.0);
    EXPECT_EQ(xs[1].t, 6.0);
}

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTangent)
{
    point origin = createPoint(0.0, 0.0, -5.0);
    vector direction = createVector(0.0, 0.0, 1.0);
    auto r = ray(origin, direction);

    point sphere_origin = createPoint(0.0, 1.0, 0.0);
    auto s = sphere(sphere_origin, 1.0);

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 5.0);
    EXPECT_EQ(xs[1].t, 5.0);
}

TEST(SphereRayIntersection, RayMissesIntersectingWithSphere)
{
    point origin = createPoint(0.0, 0.0, -5.0);
    vector direction = createVector(0.0, 0.0, 1.0);
    auto r = ray(origin, direction);

    point sphere_origin = createPoint(0.0, 2.0, 0.0);
    auto s = sphere(sphere_origin, 1.0);

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

TEST(SphereRayIntersection, RayOriginatesInsideTheSphere)
{
    point origin = createPoint(0.0, 0.0, 0.0);
    vector direction = createVector(0.0, 0.0, 1.0);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -1.0);
    EXPECT_EQ(xs[1].t, 1.0);
}

TEST(SphereRayIntersection, RayOriginatesInFrontOfTheSphere)
{
    point origin = createPoint(0.0, 0.0, 5.0);
    vector direction = createVector(0.0, 0.0, 1.0);
    auto r = ray(origin, direction);

    auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -6.0);
    EXPECT_EQ(xs[1].t, -4.0);
}

TEST(SphereRayIntersection, IntersectSetsTheObjectOnTheIntersection)
{
    auto r = ray(createPoint(0.0, 0.0, -5.0), createVector(0.0, 0.0, 1.0));
    const auto s = sphere();

    std::vector<intersection> xs = intersect(s, r);

    ASSERT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].obj, &s);
    EXPECT_EQ(xs[1].obj, &s);
}

// ---------------------------------------------------
// Sphere Transformation

TEST(SphereTransformation, ChangingSphereTranfrom)
{
    auto s = sphere();
    const auto t = translation(2.0, 3.0, 4.0);

    s.transform = t;

    EXPECT_EQ(s.transform, t);
}

// ---------------------------------------------------
// Sphere Transformation + Ray Intercept

TEST(SphereTransformation, IntersectingAScaledSphereWithARay)
{
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere();

    const auto t = scale(2.0, 2.0, 2.0);
    s.transform = t;

    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 3);
    EXPECT_EQ(xs[1].t, 7);
}

TEST(SphereTransformation, IntersectingATranslatedSphereWithARay)
{
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere();

    const auto t = translation(5.0, 0.0, 0.0);
    s.transform = t;

    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

// ---------------------------------------------------
// Sphere Normal

TEST(SphereNormal, SphereNormalAtPointOnAxisX)
{
    const auto s = sphere();
    const auto n = normalAt(s, point(1, 0, 0));
    EXPECT_EQ(vector(1, 0, 0), n);
}

TEST(SphereNormal, SphereNormalAtPointOnAxisy)
{
    const auto s = sphere();
    const auto n = normalAt(s, point(0, 1, 0));
    EXPECT_EQ(vector(0, 1, 0), n);
}

TEST(SphereNormal, SphereNormalAtPointOnAxisz)
{
    const auto s = sphere();
    const auto n = normalAt(s, point(0, 0, 1));
    EXPECT_EQ(vector(0, 0, 1), n);
}

TEST(SphereNormal, SphereNormalAtNonaxialPoint)
{
    const auto s = sphere();
    const auto n = normalAt(s, point(std::sqrt(3) / 3,
                                             std::sqrt(3) / 3,
                                             std::sqrt(3) / 3));

    EXPECT_EQ(vector(std::sqrt(3) / 3,
                     std::sqrt(3) / 3,
                     std::sqrt(3) / 3), n);
}

TEST(SphereNormal, SphereNormalIsNormalized)
{
    const auto s = sphere();
    const auto n = normalAt(s, point(std::sqrt(3) / 3,
                                             std::sqrt(3) / 3,
                                             std::sqrt(3) / 3));

    EXPECT_EQ(normalizeVector(n), n);
}

TEST(SphereNormal, SphereNormalOnTranslatedSphere)
{
    auto s = sphere();
    s.set_transform(translation(0, 1, 0));

    const auto n = normalAt(s, point(0, 1.70711, -0.70711));
    EXPECT_EQ(vector(0, 0.70711, -0.70711), n);
}

TEST(SphereNormal, SphereNormalOnATransformedSphere)
{
    auto s = sphere();
    const auto m = scale(1, 0.5, 1) * rotation_z(std::numbers::pi / 5.0);
    s.set_transform(m);

    const auto n = normalAt(s, point(0, std::sqrt(2.0) / 2.0, -std::sqrt(2.0) / 2.0));
    EXPECT_EQ(vector(0, 0.97014, -0.24254), n);
}
