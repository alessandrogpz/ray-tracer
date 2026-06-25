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
import rt.materials;

using namespace rt;

// --------------------------------------------------
// Sphere Creation

TEST(SphereCreation, DefaultSphere)
{
    const auto s = Sphere();

    EXPECT_EQ(s.get_transform(), identity());
    EXPECT_EQ(s.origin, Point(0.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(s.radius, 1.0);
}

TEST(SphereCreation, UserCreatedSphere)
{
    auto s = Sphere(Point(1.0, 2.0, 3.0), 2.5);
    s.set_transform(translation(3.0, 4.0, 5.0));

    EXPECT_EQ(s.origin, Point(1.0, 2.0, 3.0));
    EXPECT_EQ(s.get_transform(), translation(3.0, 4.0, 5.0));
    EXPECT_DOUBLE_EQ(s.radius, 2.5);
}

// --------------------------------------------------
// Ray Intersection With Object

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTwoPoints)
{
    Point origin = createPoint(0.0, 0.0, -5.0);
    Vector direction = createVector(0.0, 0.0, 1.0);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 4.0);
    EXPECT_EQ(xs[1].t, 6.0);
}

TEST(SphereRayIntersection, IntersectingRayWithSphereAtTangent)
{
    Point origin = createPoint(0.0, 0.0, -5.0);
    Vector direction = createVector(0.0, 0.0, 1.0);
    auto r = Ray(origin, direction);

    Point sphere_origin = createPoint(0.0, 1.0, 0.0);
    auto s = Sphere(sphere_origin, 1.0);

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 5.0);
    EXPECT_EQ(xs[1].t, 5.0);
}

TEST(SphereRayIntersection, RayMissesIntersectingWithSphere)
{
    Point origin = createPoint(0.0, 0.0, -5.0);
    Vector direction = createVector(0.0, 0.0, 1.0);
    auto r = Ray(origin, direction);

    Point sphere_origin = createPoint(0.0, 2.0, 0.0);
    auto s = Sphere(sphere_origin, 1.0);

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 0);
}

TEST(SphereRayIntersection, RayOriginatesInsideTheSphere)
{
    Point origin = createPoint(0.0, 0.0, 0.0);
    Vector direction = createVector(0.0, 0.0, 1.0);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -1.0);
    EXPECT_EQ(xs[1].t, 1.0);
}

TEST(SphereRayIntersection, RayOriginatesInFrontOfTheSphere)
{
    Point origin = createPoint(0.0, 0.0, 5.0);
    Vector direction = createVector(0.0, 0.0, 1.0);
    auto r = Ray(origin, direction);

    auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, -6.0);
    EXPECT_EQ(xs[1].t, -4.0);
}

TEST(SphereRayIntersection, IntersectSetsTheObjectOnTheIntersection)
{
    auto r = Ray(createPoint(0.0, 0.0, -5.0), createVector(0.0, 0.0, 1.0));
    const auto s = Sphere();

    std::vector<Intersection> xs = intersect(s, r);

    ASSERT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].obj, &s);
    EXPECT_EQ(xs[1].obj, &s);
}

// ---------------------------------------------------
// Sphere Transformation

TEST(SphereTransformation, ChangingSphereTranfrom)
{
    auto s = Sphere();
    const auto t = translation(2.0, 3.0, 4.0);

    s.set_transform(t);

    EXPECT_EQ(s.get_transform(), t);
}

// ---------------------------------------------------
// Sphere Transformation + Ray Intercept

TEST(SphereTransformation, IntersectingAScaledSphereWithARay)
{
    auto r = Ray(Point(0.0, 0.0, -5.0), Vector(0.0, 0.0, 1.0));
    auto s = Sphere();

    const auto t = scale(2.0, 2.0, 2.0);
    s.set_transform(t);

    const auto xs = intersect(s, r);

    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t, 3);
    EXPECT_EQ(xs[1].t, 7);
}

TEST(SphereTransformation, IntersectingATranslatedSphereWithARay)
{
    auto r = Ray(Point(0.0, 0.0, -5.0), Vector(0.0, 0.0, 1.0));
    auto s = Sphere();

    const auto t = translation(5.0, 0.0, 0.0);
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

    const auto n = normalAt(s, Point(0, 1.70711, -0.70711));
    EXPECT_EQ(Vector(0, 0.70711, -0.70711), n);
}

TEST(SphereNormal, SphereNormalOnATransformedSphere)
{
    auto s = Sphere();
    const auto m = scale(1, 0.5, 1) * rotation_z(std::numbers::pi / 5.0);
    s.set_transform(m);

    const auto n = normalAt(s, Point(0, std::sqrt(2.0) / 2.0, -std::sqrt(2.0) / 2.0));
    EXPECT_EQ(Vector(0, 0.97014, -0.24254), n);
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
    m.ambient = 1.0;
    s.material = m;

    EXPECT_EQ(s.material, m);
}


