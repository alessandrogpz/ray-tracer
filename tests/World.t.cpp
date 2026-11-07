#include <gtest/gtest.h>

import std;

import rt.tuple;
import rt.world;
import rt.sphere;
import rt.lights;
import rt.colors;
import rt.transformations;
import rt.ray;
import rt.intersection;
import rt.utils;
import rt.patterns;
import rt.plane;

using namespace rt;

// ---------------------------------------------------
// World Creation

TEST(WorldCreation, EmptyWorld)
{
    const World w1;

    EXPECT_EQ(w1.sphere_origins.empty(), true);
    EXPECT_EQ(w1.light, PointLight());
}

TEST(WorldCreation, DefaultWorld)
{
    PointLight expected_light(Point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Sphere s1;
    s1.material.color = Color(0.8f, 1.0f, 0.6f);
    s1.material.diffuse = 0.7f;
    s1.material.specular = 0.2f;

    Sphere s2;
    s2.set_transform(scale(0.5f, 0.5f, 0.5f));

    World w = default_world();

    EXPECT_EQ(w.light, expected_light);

    ASSERT_EQ(w.sphere_origins.size(), 2);
    // Check s1 properties
    EXPECT_EQ(w.sphere_materials[0].color, Color(0.8f, 1.0f, 0.6f));
    EXPECT_EQ(w.sphere_materials[0].diffuse, 0.7f);
    EXPECT_EQ(w.sphere_materials[0].specular, 0.2f);

    // Check s2 properties
    EXPECT_EQ(w.sphere_transforms[1], scale(0.5f, 0.5f, 0.5f));
}

// ---------------------------------------------------
// Intersecting World

TEST(IntersectingWorld, IntersectWorldWithRay)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));

    const std::vector<Intersection>xs = intersect_world(w, r);

    EXPECT_EQ(xs.size(), 4);
    EXPECT_TRUE(equal(xs[0].t, 4.0f));
    EXPECT_TRUE(equal(xs[1].t, 4.5f));
    EXPECT_TRUE(equal(xs[2].t, 5.5f));
    EXPECT_TRUE(equal(xs[3].t, 6.0f));
}

// ---------------------------------------------------
// Precomputing the State of an Intersection

TEST(PrecomputingInteraction, PrecomputeStateOfIntersection)
{
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Sphere s;
    World w;
    w.add_sphere(s);
    const Intersection i(4.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_FLOAT_EQ(comps.intersection.t, i.t);
    EXPECT_EQ(comps.intersection.shape_index, i.shape_index);
    EXPECT_EQ(comps.intersection.shape_type, i.shape_type);

    EXPECT_EQ(comps.point, Point(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(comps.eye_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(comps.normal_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_FALSE(comps.inside);
}

TEST(PrecomputingInteraction, PrecomputeStateOfIntersectionInside)
{
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const Sphere s;
    World w;
    w.add_sphere(s);
    const Intersection i(1.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_FLOAT_EQ(comps.intersection.t, i.t);
    EXPECT_EQ(comps.point, Point(0.0f, 0.0f, 1.0f));
    EXPECT_EQ(comps.eye_v, Vector(0.0f, 0.0f, -1.0f));
    EXPECT_TRUE(comps.inside);
    // The normal vector is inverted since it was pointing outwards
    EXPECT_EQ(comps.normal_v, Vector(0.0f, 0.0f, -1.0f));
}

TEST(PrecomputingInteraction, TheHitShouldOffsetThePoint)
{
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    Sphere s;
    s.set_transform(translation(0.0f, 0.0f, 1.0f));
    World w;
    w.add_sphere(s);
    const Intersection i(5.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);

    EXPECT_LT(comps.over_point.z, -EPSILON / 2.0f);
    EXPECT_GT(comps.point.z, comps.over_point.z);
}

// ---------------------------------------------------
// Shading an Intersection

TEST(ShadingIntersection, ShadingIntersectionFormOutside)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(4.0f, 0, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);
    const Color c = shade_hit(w, comps);

    EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f));
}

TEST(ShadingIntersection, ShadingIntersectionFormInside)
{
    World w = default_world();
    w.light = PointLight(Point(0.0f, 0.25f, 0.0f), Color(1.0f, 1.0f, 1.0f));
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(0.5f, 1, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);
    const Color c = shade_hit(w, comps);

    EXPECT_EQ(c, Color(0.90498f, 0.90498f, 0.90498f));
}

TEST(ShadingIntersection, ShadingIntersectionInShadow)
{
    World w;
    w.light = PointLight(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Sphere s1;
    w.add_sphere(s1);

    Sphere s2;
    s2.set_transform(translation(0.0f, 0.0f, 10.0f));
    w.add_sphere(s2);

    const Ray r(Point(0.0f, 0.0f, 5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(4.0f, 1, ShapeType::Sphere);
    const Comp comps = prepare_computation(i, r, w);
    const Color c = shade_hit(w, comps);

    EXPECT_EQ(c, Color(0.1f, 0.1f, 0.1f));
}

// ---------------------------------------------------
// Color At a Ray

TEST(ColorAtRay, ColorWhenRayMisses)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 1.0f, 0.0f));
    const Color c = color_at(w, r);

    EXPECT_EQ(c, Color(0.0f, 0.0f, 0.0f));
}

TEST(ColorAtRay, ColorWhenRayHits)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Color c = color_at(w, r);

    EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f));
}

TEST(ColorAtRay, ColorWithIntersectionBehindRay)
{
    World w = default_world();
    
    // Set ambient to 1 for both spheres so they glow completely in their own color
    w.sphere_materials[0].ambient = 1.0f;
    w.sphere_materials[1].ambient = 1.0f;

    // Ray originates inside the outer sphere but points at the inner sphere from its edge
    const Ray r(Point(0.0f, 0.0f, 0.75f), Vector(0.0f, 0.0f, -1.0f));
    const Color c = color_at(w, r);

    // Should return the color of the inner sphere
    EXPECT_EQ(c, w.sphere_materials[1].color);
}

// ---------------------------------------------------
// Shadows

TEST(Shadows, NoShadowWhenPointIsDirectlyVisible)
{
    const World w = default_world();
    const Point p(0.0f, 10.0f, 0.0f);
    const bool has_shadow = is_shadowed(w, p);

    EXPECT_FALSE(has_shadow);
}

TEST(Shadows, ShadowWhenObjectBetweenPointAndLight)
{
    const World w = default_world();
    const Point p(10.0f, -10.0f, 10.0f);
    const bool has_shadow = is_shadowed(w, p);

    EXPECT_TRUE(has_shadow);
}

TEST(Shadows, NoShadowWhenObjectBehindLight)
{
    const World w = default_world();
    const Point p(-20.0f, 20.0f, -20.0f);
    const bool has_shadow = is_shadowed(w, p);

    EXPECT_FALSE(has_shadow);
}

TEST(Shadows, NoShadowWhenObjectBehindPoint)
{
    const World w = default_world();
    const Point p(-2.0f, 2.0f, -2.0f);
    const bool has_shadow = is_shadowed(w, p);

    EXPECT_FALSE(has_shadow);
}

// ---------------------------------------------------
// Patterns in the World

TEST(WorldPatterns, ShadeHitPlacesPatternsWithTheShapeTransform)
{
    World w;
    w.light = PointLight(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    // A sphere scaled 2x: world x = 1.5 is object x = 0.75, inside the first (white) stripe.
    // Without the object transform, x = 1.5 would fall in the second (black) stripe.
    Sphere s;
    s.set_transform(scale(2.0f, 2.0f, 2.0f));
    s.material.pattern = stripe_pattern(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f));
    s.material.ambient = 1.0f;
    s.material.diffuse = 0.0f;
    s.material.specular = 0.0f;
    w.add_sphere(s);

    const Ray r(Point(1.5f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));

    EXPECT_EQ(color_at(w, r), Color(1.0f, 1.0f, 1.0f));
}

// ---------------------------------------------------
// Reflection

constexpr float SQRT2 = std::numbers::sqrt2_v<float>;

TEST(Reflection, PrecomputingTheReflectionVector)
{
    World w;
    w.add_plane(Plane());
    const Ray r(Point(0.0f, 1.0f, -1.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
    const Intersection i(SQRT2, 0, ShapeType::Plane);

    const Comp comps = prepare_computation(i, r, w);

    EXPECT_EQ(comps.reflect_v, Vector(0.0f, SQRT2 / 2.0f, SQRT2 / 2.0f));
}

TEST(Reflection, TheReflectedColorForANonreflectiveMaterial)
{
    World w = default_world();
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f));
    w.sphere_materials[1].ambient = 1.0f;
    const Intersection i(1.0f, 1, ShapeType::Sphere);

    const Comp comps = prepare_computation(i, r, w);
    const Color color = reflected_color(w, comps);

    EXPECT_EQ(color, Color(0.0f, 0.0f, 0.0f));
}

TEST(Reflection, TheReflectedColorForAReflectiveMaterial)
{
    World w = default_world();
    Plane shape;
    shape.material.reflective = 0.5f;
    shape.set_transform(translation(0.0f, -1.0f, 0.0f));
    w.add_plane(shape);
    const Ray r(Point(0.0f, 0.0f, -3.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
    const Intersection i(SQRT2, 0, ShapeType::Plane);

    const Comp comps = prepare_computation(i, r, w);
    const Color color = reflected_color(w, comps);

    EXPECT_EQ(color, Color(0.19032f, 0.2379f, 0.14274f));
}

TEST(Reflection, ShadeHitWithAReflectiveMaterial)
{
    World w = default_world();
    Plane shape;
    shape.material.reflective = 0.5f;
    shape.set_transform(translation(0.0f, -1.0f, 0.0f));
    w.add_plane(shape);
    const Ray r(Point(0.0f, 0.0f, -3.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
    const Intersection i(SQRT2, 0, ShapeType::Plane);

    const Comp comps = prepare_computation(i, r, w);
    const Color color = shade_hit(w, comps);

    EXPECT_EQ(color, Color(0.87677f, 0.92436f, 0.82918f));
}

TEST(Reflection, ColorAtWithMutuallyReflectiveSurfaces)
{
    World w;
    w.light = PointLight(Point(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f));

    Plane lower;
    lower.material.reflective = 1.0f;
    lower.set_transform(translation(0.0f, -1.0f, 0.0f));
    w.add_plane(lower);

    Plane upper;
    upper.material.reflective = 1.0f;
    upper.set_transform(translation(0.0f, 1.0f, 0.0f));
    w.add_plane(upper);

    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));

    // Terminates (instead of overflowing the stack) because recursion is bounded
    const Color c = color_at(w, r);
    EXPECT_GT(c.r, 0.0f);
}

TEST(Reflection, TheReflectedColorAtTheMaximumRecursiveDepth)
{
    World w = default_world();
    Plane shape;
    shape.material.reflective = 0.5f;
    shape.set_transform(translation(0.0f, -1.0f, 0.0f));
    w.add_plane(shape);
    const Ray r(Point(0.0f, 0.0f, -3.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
    const Intersection i(SQRT2, 0, ShapeType::Plane);

    const Comp comps = prepare_computation(i, r, w);
    const Color color = reflected_color(w, comps, 0);

    EXPECT_EQ(color, Color(0.0f, 0.0f, 0.0f));
}

// ---------------------------------------------------
// Transparency and Refraction

TEST(Refraction, FindingN1AndN2AtVariousIntersections)
{
    World w;

    Sphere a = glass_sphere();
    a.set_transform(scale(2.0f, 2.0f, 2.0f));
    a.material.refractive_index = 1.5f;
    w.add_sphere(a);

    Sphere b = glass_sphere();
    b.set_transform(translation(0.0f, 0.0f, -0.25f));
    b.material.refractive_index = 2.0f;
    w.add_sphere(b);

    Sphere c = glass_sphere();
    c.set_transform(translation(0.0f, 0.0f, 0.25f));
    c.material.refractive_index = 2.5f;
    w.add_sphere(c);

    const Ray r(Point(0.0f, 0.0f, -4.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = {
        Intersection(2.0f, 0, ShapeType::Sphere),
        Intersection(2.75f, 1, ShapeType::Sphere),
        Intersection(3.25f, 2, ShapeType::Sphere),
        Intersection(4.75f, 1, ShapeType::Sphere),
        Intersection(5.25f, 2, ShapeType::Sphere),
        Intersection(6.0f, 0, ShapeType::Sphere),
    };

    struct Example { std::size_t index; float n1; float n2; };
    const std::array<Example, 6> examples = {{
        {0, 1.0f, 1.5f},
        {1, 1.5f, 2.0f},
        {2, 2.0f, 2.5f},
        {3, 2.5f, 2.5f},
        {4, 2.5f, 1.5f},
        {5, 1.5f, 1.0f},
    }};

    for (const Example& e : examples)
    {
        const Comp comps = prepare_computation(xs[e.index], r, w, xs);
        EXPECT_FLOAT_EQ(comps.n1, e.n1) << "at intersection " << e.index;
        EXPECT_FLOAT_EQ(comps.n2, e.n2) << "at intersection " << e.index;
    }
}

TEST(Refraction, TheUnderPointIsOffsetBelowTheSurface)
{
    World w;
    Sphere shape = glass_sphere();
    shape.set_transform(translation(0.0f, 0.0f, 1.0f));
    w.add_sphere(shape);
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const Intersection i(5.0f, 0, ShapeType::Sphere);
    const std::vector<Intersection> xs = {i};

    const Comp comps = prepare_computation(i, r, w, xs);

    EXPECT_GT(comps.under_point.z, EPSILON / 2.0f);
    EXPECT_LT(comps.point.z, comps.under_point.z);
}

TEST(Refraction, TheRefractedColorWithAnOpaqueSurface)
{
    const World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = {
        Intersection(4.0f, 0, ShapeType::Sphere),
        Intersection(6.0f, 0, ShapeType::Sphere),
    };

    const Comp comps = prepare_computation(xs[0], r, w, xs);
    const Color c = refracted_color(w, comps, 5);

    EXPECT_EQ(c, Color(0.0f, 0.0f, 0.0f));
}

TEST(Refraction, TheRefractedColorAtTheMaximumRecursiveDepth)
{
    World w = default_world();
    w.sphere_materials[0].transparency = 1.0f;
    w.sphere_materials[0].refractive_index = 1.5f;
    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = {
        Intersection(4.0f, 0, ShapeType::Sphere),
        Intersection(6.0f, 0, ShapeType::Sphere),
    };

    const Comp comps = prepare_computation(xs[0], r, w, xs);
    const Color c = refracted_color(w, comps, 0);

    EXPECT_EQ(c, Color(0.0f, 0.0f, 0.0f));
}

TEST(Refraction, TheRefractedColorUnderTotalInternalReflection)
{
    World w = default_world();
    w.sphere_materials[0].transparency = 1.0f;
    w.sphere_materials[0].refractive_index = 1.5f;
    const Ray r(Point(0.0f, 0.0f, SQRT2 / 2.0f), Vector(0.0f, 1.0f, 0.0f));
    const std::vector<Intersection> xs = {
        Intersection(-SQRT2 / 2.0f, 0, ShapeType::Sphere),
        Intersection(SQRT2 / 2.0f, 0, ShapeType::Sphere),
    };

    // Inside the sphere, so the hit is the second intersection
    const Comp comps = prepare_computation(xs[1], r, w, xs);
    const Color c = refracted_color(w, comps, 5);

    EXPECT_EQ(c, Color(0.0f, 0.0f, 0.0f));
}

TEST(Refraction, TheRefractedColorWithARefractedRay)
{
    World w = default_world();
    w.sphere_materials[0].ambient = 1.0f;
    w.sphere_materials[0].pattern = test_pattern();
    w.sphere_materials[1].transparency = 1.0f;
    w.sphere_materials[1].refractive_index = 1.5f;
    const Ray r(Point(0.0f, 0.0f, 0.1f), Vector(0.0f, 1.0f, 0.0f));
    const std::vector<Intersection> xs = {
        Intersection(-0.9899f, 0, ShapeType::Sphere),
        Intersection(-0.4899f, 1, ShapeType::Sphere),
        Intersection(0.4899f, 1, ShapeType::Sphere),
        Intersection(0.9899f, 0, ShapeType::Sphere),
    };

    const Comp comps = prepare_computation(xs[2], r, w, xs);
    const Color c = refracted_color(w, comps, 5);

    EXPECT_EQ(c, Color(0.0f, 0.99888f, 0.04725f));
}

TEST(Refraction, ShadeHitWithATransparentMaterial)
{
    World w = default_world();

    Plane floor_plane;
    floor_plane.set_transform(translation(0.0f, -1.0f, 0.0f));
    floor_plane.material.transparency = 0.5f;
    floor_plane.material.refractive_index = 1.5f;
    w.add_plane(floor_plane);

    Sphere ball;
    ball.material.color = Color(1.0f, 0.0f, 0.0f);
    ball.material.ambient = 0.5f;
    ball.set_transform(translation(0.0f, -3.5f, -0.5f));
    w.add_sphere(ball);

    const Ray r(Point(0.0f, 0.0f, -3.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));
    const std::vector<Intersection> xs = {Intersection(SQRT2, 0, ShapeType::Plane)};

    const Comp comps = prepare_computation(xs[0], r, w, xs);
    const Color color = shade_hit(w, comps, 5);

    EXPECT_EQ(color, Color(0.93642f, 0.68642f, 0.68642f));
}

// ---------------------------------------------------
// Fresnel Effect

TEST(Fresnel, TheSchlickApproximationUnderTotalInternalReflection)
{
    World w;
    w.add_sphere(glass_sphere());
    const Ray r(Point(0.0f, 0.0f, SQRT2 / 2.0f), Vector(0.0f, 1.0f, 0.0f));
    const std::vector<Intersection> xs = {
        Intersection(-SQRT2 / 2.0f, 0, ShapeType::Sphere),
        Intersection(SQRT2 / 2.0f, 0, ShapeType::Sphere),
    };

    const Comp comps = prepare_computation(xs[1], r, w, xs);
    const float reflectance = schlick(comps);

    EXPECT_FLOAT_EQ(reflectance, 1.0f);
}

TEST(Fresnel, TheSchlickApproximationWithAPerpendicularViewingAngle)
{
    World w;
    w.add_sphere(glass_sphere());
    const Ray r(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
    const std::vector<Intersection> xs = {
        Intersection(-1.0f, 0, ShapeType::Sphere),
        Intersection(1.0f, 0, ShapeType::Sphere),
    };

    const Comp comps = prepare_computation(xs[1], r, w, xs);
    const float reflectance = schlick(comps);

    EXPECT_TRUE(equal(reflectance, 0.04f));
}

TEST(Fresnel, TheSchlickApproximationWithSmallAngleAndN2GreaterThanN1)
{
    World w;
    w.add_sphere(glass_sphere());
    const Ray r(Point(0.0f, 0.99f, -2.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = {Intersection(1.8589f, 0, ShapeType::Sphere)};

    const Comp comps = prepare_computation(xs[0], r, w, xs);
    const float reflectance = schlick(comps);

    EXPECT_NEAR(reflectance, 0.48873f, 0.0005f);
}

TEST(Fresnel, ShadeHitWithAReflectiveTransparentMaterial)
{
    World w = default_world();
    const Ray r(Point(0.0f, 0.0f, -3.0f), Vector(0.0f, -SQRT2 / 2.0f, SQRT2 / 2.0f));

    Plane floor_plane;
    floor_plane.set_transform(translation(0.0f, -1.0f, 0.0f));
    floor_plane.material.reflective = 0.5f;
    floor_plane.material.transparency = 0.5f;
    floor_plane.material.refractive_index = 1.5f;
    w.add_plane(floor_plane);

    Sphere ball;
    ball.material.color = Color(1.0f, 0.0f, 0.0f);
    ball.material.ambient = 0.5f;
    ball.set_transform(translation(0.0f, -3.5f, -0.5f));
    w.add_sphere(ball);

    const std::vector<Intersection> xs = {Intersection(SQRT2, 0, ShapeType::Plane)};

    const Comp comps = prepare_computation(xs[0], r, w, xs);
    const Color color = shade_hit(w, comps, 5);

    EXPECT_EQ(color, Color(0.93391f, 0.69643f, 0.69243f));
}

// ---------------------------------------------------
// Opting Out of Shadows (the chapter's closing challenge)

TEST(Shadows, AnObjectThatDoesNotCastShadowsLetsLightThrough)
{
    World w = default_world();
    w.sphere_materials[0].casts_shadow = false;
    w.sphere_materials[1].casts_shadow = false;

    // Behind both default spheres from the light's point of view
    const Point p(10.0f, -10.0f, 10.0f);

    EXPECT_FALSE(is_shadowed(w, p));
}

TEST(Shadows, AShadowCasterBehindANonCastingObjectStillBlocksTheLight)
{
    World w = default_world();
    // Only the outer sphere opts out; the inner one still blocks the light
    w.sphere_materials[0].casts_shadow = false;

    const Point p(10.0f, -10.0f, 10.0f);

    EXPECT_TRUE(is_shadowed(w, p));
}
