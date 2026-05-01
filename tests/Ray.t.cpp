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

// -------------------------------------------------------------------
// Transforming Rays & Spheres

TEST(TransformRay, TranslatingARay)
{
    const auto r = ray(point(1.0f,2.0f,3.0f), vector(0.0f, 1.0f, 0.0f));
    const auto m = translation(3.0f, 4.0f, 5.0f);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, point(4.0f, 6.0f, 8.0f));
    EXPECT_EQ(r2.direction, vector(0.0f, 1.0f, 0.0f));
}

TEST(TransformRay, ScalingARay)
{
    const auto r = ray(point(1.0f,2.0f,3.0f), vector(0.0f, 1.0f, 0.0f));
    const auto m = scale(2.0f, 3.0f, 4.0f);

    const auto r2 = transformRay(r, m);

    EXPECT_EQ(r2.origin, point(2.0f, 6.0f, 12.0f));
    EXPECT_EQ(r2.direction, vector(0.0f, 3.0f, 0.0f));
}
