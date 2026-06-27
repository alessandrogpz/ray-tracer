#include <gtest/gtest.h>
import std;

import rt.lights;
import rt.tuple;
import rt.materials;
import rt.colors;
import rt.shading;

using namespace rt;

TEST(Lighting, EyeBetweenLightAndSurface)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);
    Vector norma_v = Vector(0.0f, 0.0f, -1.0f);
    PointLight point_light(Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(1.9f, 1.9f, 1.9f));
}

TEST(Lighting, EyeOffset45Degrees)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Vector eye_v = Vector(0.0f, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f);

    Color result = lighting(m, light, position, eye_v, normal_vector);
    EXPECT_EQ(Color(1.0f, 1.0f, 1.0f), result);
}

TEST(Lighting, LightOffset45Degrees)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);

    Color result = lighting(m, light, position, eye_v, normal_vector);
    EXPECT_EQ(Color(0.7364f, 0.7364f, 0.7364f), result);
}

TEST(Lighting, EyeInPathOfReflectionVector)
{
    Material m;
    Point position;
    Vector normal_vector = createVector(0.0f, 0.0f, -1.0f);
    PointLight light = PointLight(createPoint(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));

    Vector eye_v = Vector(0.0f, -std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f);

    Color result = lighting(m, light, position, eye_v, normal_vector);

    EXPECT_EQ(result, Color(1.6364f, 1.6364f, 1.6364f));
}

TEST(Lighting, LightBehindSurface)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0f, 0.0f, -1.0f);
    Vector norma_v = Vector(0.0f, 0.0f, -1.0f);
    PointLight point_light(Point(0.0f, 0.0f, 10.0f), Color(1.0f, 1.0f, 1.0f));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(0.1f, 0.1f, 0.1f));
}