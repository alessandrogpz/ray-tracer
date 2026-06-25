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
    Point origin = createPoint(1.0, 2.0, 3.0);
    Vector direction = createVector(4.0, 5.0, 6.0);

    Ray r = Ray(origin, direction);

    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

// ---------------------------------------------------
// Ray Operations

TEST(RayOperation, ComputingPointFromDistance)
{
    Ray r = Ray(Point(2.0, 3.0, 4.0), Vector(1.0, 0.0, 0.0));

    Point pos1 = position(r, 0);
    Point pos2 = position(r, 1);
    Point pos3 = position(r, -1);
    Point pos4 = position(r, 2.5);

    EXPECT_EQ(pos1, Point(2.0, 3.0, 4.0));
    EXPECT_EQ(pos2, Point(3.0, 3.0, 4.0));
    EXPECT_EQ(pos3, Point(1.0, 3.0, 4.0));
    EXPECT_EQ(pos4, Point(4.5, 3.0, 4.0));
}

// -------------------------------------------------------------------
// Transforming Rays & Spheres

TEST(TransformRay, TranslatingARay)
{
    const auto r = Ray(Point(1.0,2.0,3.0), Vector(0.0, 1.0, 0.0));
    const auto m = translation(3.0, 4.0, 5.0);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, Point(4.0, 6.0, 8.0));
    EXPECT_EQ(r2.direction, Vector(0.0, 1.0, 0.0));
}

TEST(TransformRay, ScalingARay)
{
    const auto r = Ray(Point(1.0,2.0,3.0), Vector(0.0, 1.0, 0.0));
    const auto m = scale(2.0, 3.0, 4.0);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, Point(2.0, 6.0, 12.0));
    EXPECT_EQ(r2.direction, Vector(0.0, 3.0, 0.0));
}
