#include <gtest/gtest.h>

import rt.intersection;
import rt.shapes;
import rt.ray;

using namespace rt;

// ---------------------------------------------------------
// Creating a Intersection

TEST(IntersectionCreation, AnIntersectionEncapsulatesTAndObject)
{
    auto s = Sphere();
    auto i = Intersection(3.5, &s);

    EXPECT_DOUBLE_EQ(i.t, 3.5);
    EXPECT_EQ(i.obj, &s);
}

//-------------------------------------------------------------
// Aggregating multiple intersections into a standard Vector.

TEST(AggregatingIntersections, IntersectionSetValidation)
{
    const auto s = Sphere();
    auto i1 = Intersection(1.0, &s);
    auto i2 = Intersection(2.0, &s);

    std::vector<Intersection> xs = {i1, i2};

    EXPECT_EQ(xs.size(), 2);
    EXPECT_DOUBLE_EQ(xs[0].t, 1.0);
    EXPECT_DOUBLE_EQ(xs[1].t, 2.0);
}

//-------------------------------------------------------------
// Valid Intersection Tests

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHavePositiveT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(1.0, &s);
    const auto i2 = Intersection(2.0, &s);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_DOUBLE_EQ(i->t, i1.t);
    EXPECT_EQ(i->obj, i1.obj);
}

TEST(SphereIntersectionTests, HitWhenSomeIntersectionsHaveNegativeT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(-1.0, &s);
    const auto i2 = Intersection(2.0, &s);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_DOUBLE_EQ(i->t, i2.t);
    EXPECT_EQ(i->obj, i2.obj);
}

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHaveNegativeT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(-2.0, &s);
    const auto i2 = Intersection(-1.0, &s);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FALSE(i);
}

TEST(SphereIntersectionTests, HitTheLowestNonnegativeIntersection)
{
    const auto s = Sphere();
    const auto i1 = Intersection(5.0, &s);
    const auto i2 = Intersection(7.0, &s);
    const auto i3 = Intersection(-3.0, &s);
    const auto i4 = Intersection(2.0, &s);

    std::vector<Intersection> xs = {i1, i2, i3, i4};

    const auto i = hit(xs);

    EXPECT_DOUBLE_EQ(i->t, i4.t);
    EXPECT_EQ(i->obj, i4.obj);
}