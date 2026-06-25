#include <gtest/gtest.h>
#include <cmath>

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
    Vector eye_v = Vector(0.0, 0.0, -1.0);
    Vector norma_v = Vector(0.0, 0.0, -1.0);
    PointLight point_light(Point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(1.9, 1.9, 1.9));
}

TEST(Lighting, EyeOffset45Degrees)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0, sqrt(2) / 2, -sqrt(2) / 2);
    Vector norma_v = Vector(0.0, 0.0, -1.0);
    PointLight point_light(Point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(1.0, 1.0, 1.0));
}

TEST(Lighting, LightOffset45Degrees)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0, 0.0, -1.0);
    Vector norma_v = Vector(0.0, 0.0, -1.0);
    PointLight point_light(Point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(0.7364, 0.7364, 0.7364));
}

TEST(Lighting, EyeInPathOfReflectionVector)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0, -sqrt(2) / 2, -sqrt(2) / 2);
    Vector norma_v = Vector(0.0, 0.0, -1.0);
    PointLight point_light(Point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(1.6364, 1.6364, 1.6364));
}

TEST(Lighting, LightBehindSurface)
{
    Material m;
    Point position;
    Vector eye_v = Vector(0.0, 0.0, -1.0);
    Vector norma_v = Vector(0.0, 0.0, -1.0);
    PointLight point_light(Point(0.0, 0.0, 10.0), Color(1.0, 1.0, 1.0));

    Color result = lighting(m, point_light, position, eye_v, norma_v );

    EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
}