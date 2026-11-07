#include <gtest/gtest.h>
import std;

import rt.lights;
import rt.tuple;
import rt.materials;
import rt.colors;
import rt.shading;
import rt.patterns;
import rt.sphere;

using namespace rt;

// ---------------------------------------------------
// Lighting, Eye, Surface Positions

TEST(Lighting, EyeBetweenLightAndSurface)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);
    Vector norma_v = Vector(0.0f, 0.0f, -1.0f);
    PointLight point_light(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = false;

    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), point_light, position, eye_v, norma_v, inShadow );

    EXPECT_EQ(result, Color(1.9f, 1.9f, 1.9f));
}

TEST(Lighting, EyeOffset45Degrees)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = false;

    Vector eye_v = Vector(0.0f, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f);
    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), light, position, eye_v, normal_vector, inShadow);
    EXPECT_EQ(Color(1.0f, 1.0f, 1.0f), result);
}

TEST(Lighting, LightOffset45Degrees)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = false;

    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);

    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), light, position, eye_v, normal_vector, inShadow);
    EXPECT_EQ(Color(0.7364f, 0.7364f, 0.7364f), result);
}

TEST(Lighting, EyeInPathOfReflectionVector)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = false;

    Vector eye_v = Vector(0.0f, -std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f);

    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), light, position, eye_v, normal_vector, inShadow);

    EXPECT_EQ(result, Color(1.6364f, 1.6364f, 1.6364f));
}

TEST(Lighting, LightBehindSurface)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);
    Vector norma_v = Vector(0.0f, 0.0f, -1.0f);
    PointLight point_light(Point(0.0f, 0.0f, 10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = false;

    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), point_light, position, eye_v, norma_v, inShadow);

    EXPECT_EQ(result, Color(0.1f, 0.1f, 0.1f));
}

// ---------------------------------------------------
// Shadow

TEST(Shadow, LightingWithSurfaceInShadow)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);
    Vector norma_v = Vector(0.0f, 0.0f, -1.0f);
    PointLight point_light(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    bool inShadow = true;

    const Sphere object;
    Color result = lighting(m, object.get_transform_inverse(), point_light, position, eye_v, norma_v, inShadow );

    EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
}

// ---------------------------------------------------
// Lighting with Patterns

TEST(Lighting, LightingWithAPattern)
{
    Material m;

    m.pattern = stripe_pattern(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f));
    m.ambient = 1.0f;
    m.diffuse = 0.0f;
    m.specular = 0.0f;
    const Vector eye_v(0.0f, 0.0f, -1.0f);
    const Vector normal_v(0.0f, 0.0f, -1.0f);
    const PointLight light(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    const Sphere object;
    const Color c1 = lighting(m, object.get_transform_inverse(), light, Point(0.9f, 0.0f, 0.0f), eye_v, normal_v, false);
    const Color c2 = lighting(m, object.get_transform_inverse(), light, Point(1.1f, 0.0f, 0.0f), eye_v, normal_v, false);

    EXPECT_EQ(c1, Color(1.0f, 1.0f, 1.0f));
    EXPECT_EQ(c2, Color(0.0f, 0.0f, 0.0f));
}
