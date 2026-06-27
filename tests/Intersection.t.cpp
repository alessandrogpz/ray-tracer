#include <gtest/gtest.h>

import rt.intersection;
import rt.sphere;
import rt.ray;

using namespace rt;

// ---------------------------------------------------------
// Creating a Intersection

TEST(IntersectionCreation, AnIntersectionEncapsulatesTAndObject)
{
    auto s = Sphere();
    auto i = Intersection(3.5f, 0, ShapeType::Sphere);

    EXPECT_FLOAT_EQ(i.t, 3.5f);
    EXPECT_EQ(i.shape_index, 0);
    EXPECT_EQ(i.shape_type, ShapeType::Sphere);
}

//-------------------------------------------------------------
// Aggregating multiple intersections into a standard Vector.

TEST(AggregatingIntersections, IntersectionSetValidation)
{
    const auto s = Sphere();
    auto i1 = Intersection(1.0f, 0, ShapeType::Sphere);
    auto i2 = Intersection(2.0f, 0, ShapeType::Sphere);

    std::vector<Intersection> xs = {i1, i2};

    EXPECT_EQ(xs.size(), 2);
    EXPECT_FLOAT_EQ(xs[0].t, 1.0f);
    EXPECT_FLOAT_EQ(xs[1].t, 2.0f);
}

//-------------------------------------------------------------
// Valid Intersection Tests

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHavePositiveT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(1.0f, 0, ShapeType::Sphere);
    const auto i2 = Intersection(2.0f, 0, ShapeType::Sphere);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i1.t);
    EXPECT_EQ(i->shape_index, i1.shape_index);
    EXPECT_EQ(i->shape_type, i1.shape_type);
}

TEST(SphereIntersectionTests, HitWhenSomeIntersectionsHaveNegativeT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(-1.0f, 0, ShapeType::Sphere);
    const auto i2 = Intersection(2.0f, 0, ShapeType::Sphere);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i2.t);
    EXPECT_EQ(i->shape_index, i2.shape_index);
    EXPECT_EQ(i->shape_type, i2.shape_type);
}

TEST(SphereIntersectionTests, HitWhenAllIntersectionsHaveNegativeT)
{
    const auto s = Sphere();
    const auto i1 = Intersection(-2.0f, 0, ShapeType::Sphere);
    const auto i2 = Intersection(-1.0f, 0, ShapeType::Sphere);

    std::vector<Intersection> xs = {i1, i2};

    const auto i = hit(xs);

    EXPECT_FALSE(i);
}

TEST(SphereIntersectionTests, HitTheLowestNonnegativeIntersection)
{
    const auto s = Sphere();
    const auto i1 = Intersection(5.0f, 0, ShapeType::Sphere);
    const auto i2 = Intersection(7.0f, 0, ShapeType::Sphere);
    const auto i3 = Intersection(-3.0f, 0, ShapeType::Sphere);
    const auto i4 = Intersection(2.0f, 0, ShapeType::Sphere);

    std::vector<Intersection> xs = {i1, i2, i3, i4};

    const auto i = hit(xs);

    EXPECT_FLOAT_EQ(i->t, i4.t);
    EXPECT_EQ(i->shape_index, i4.shape_index);
    EXPECT_EQ(i->shape_type, i4.shape_type);
}