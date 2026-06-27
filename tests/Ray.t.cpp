#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.sphere;
import rt.intersection;

using namespace rt;

// ---------------------------------------------------
// Creating a Ray

TEST(RayCreation, CreatingAndQueryingARay)
{
    Point origin = createPoint(1.0f, 2.0f, 3.0f);
    Vector direction = createVector(4.0f, 5.0f, 6.0f);

    Ray r = Ray(origin, direction);

    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

// ---------------------------------------------------
// Ray Operations

TEST(RayOperation, ComputingPointFromDistance)
{
    Ray r = Ray(Point(2.0f, 3.0f, 4.0f), Vector(1.0f, 0.0f, 0.0f));

    Point pos1 = position(r, 0);
    Point pos2 = position(r, 1);
    Point pos3 = position(r, -1);
    Point pos4 = position(r, 2.5f);

    EXPECT_EQ(pos1, Point(2.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos2, Point(3.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos3, Point(1.0f, 3.0f, 4.0f));
    EXPECT_EQ(pos4, Point(4.5f, 3.0f, 4.0f));
}

// -------------------------------------------------------------------
// Transforming Rays & Spheres

TEST(TransformRay, TranslatingARay)
{
    const auto r = Ray(Point(1.0f,2.0f,3.0f), Vector(0.0f, 1.0f, 0.0f));
    const auto m = translation(3.0f, 4.0f, 5.0f);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, Point(4.0f, 6.0f, 8.0f));
    EXPECT_EQ(r2.direction, Vector(0.0f, 1.0f, 0.0f));
}

TEST(TransformRay, ScalingARay)
{
    const auto r = Ray(Point(1.0f,2.0f,3.0f), Vector(0.0f, 1.0f, 0.0f));
    const auto m = scale(2.0f, 3.0f, 4.0f);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, Point(2.0f, 6.0f, 12.0f));
    EXPECT_EQ(r2.direction, Vector(0.0f, 3.0f, 0.0f));
}
