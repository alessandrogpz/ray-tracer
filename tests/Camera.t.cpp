#include <gtest/gtest.h>

import rt.camera;
import rt.tuple;
import rt.matrix;
import rt.ray;
import rt.canvas;
import rt.world;
import rt.colors;
import rt.transformations;

using namespace rt;

const float PI = 3.14159265f;

// 1. Constructing a camera
TEST(CameraTests, ConstructingACamera)
{
    std::size_t hsize = 160;
    std::size_t vsize = 120;
    float fov = PI / 2.0f;

    Camera c(hsize, vsize, fov);

    EXPECT_EQ(c.hsize, 160);
    EXPECT_EQ(c.vsize, 120);
    EXPECT_FLOAT_EQ(c.field_of_view, PI / 2.0f);
    EXPECT_EQ(c.transform, identity());
}

// 2. The pixel size for a horizontal canvas
TEST(CameraTests, PixelSizeForHorizontalCanvas)
{
    Camera c(200, 125, PI / 2.0f);
    EXPECT_FLOAT_EQ(c.pixel_size, 0.01f);
}

// 3. The pixel size for a vertical canvas
TEST(CameraTests, PixelSizeForVerticalCanvas)
{
    Camera c(125, 200, PI / 2.0f);
    EXPECT_FLOAT_EQ(c.pixel_size, 0.01f);
}

// 4. Constructing a ray through the center of the canvas
TEST(CameraTests, RayThroughCenterOfCanvas)
{
    Camera c(201, 101, PI / 2.0f);
    Ray r = ray_for_pixel(c, 100, 50);

    EXPECT_EQ(r.origin, Point(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(r.direction, Vector(0.0f, 0.0f, -1.0f));
}

// 5. Constructing a ray through a corner of the canvas
TEST(CameraTests, RayThroughCornerOfCanvas)
{
    Camera c(201, 101, PI / 2.0f);
    Ray r = ray_for_pixel(c, 0, 0);

    EXPECT_EQ(r.origin, Point(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(r.direction, Vector(0.66519f, 0.33259f, -0.66851f));
}

// 6. Constructing a ray when the camera is transformed
TEST(CameraTests, RayWithTransformedCamera)
{
    Camera c(201, 101, PI / 2.0f);
    c.set_transform(rotation_y(PI / 4.0f) * translation(0.0f, 2.0f, -5.0f));
    Ray r = ray_for_pixel(c, 100, 50);

    EXPECT_EQ(r.origin, Point(0.0f, -2.0f, 5.0f));
    EXPECT_EQ(r.direction, Vector(0.70711f, 0.0f, -0.70711f));
}

// 7. Rendering a world with a camera
TEST(CameraTests, RenderingWorldWithCamera)
{
    World w = default_world();
    Camera c(11, 11, PI / 2.0f);
    Point from(0.0f, 0.0f, -5.0f);
    Point to(0.0f, 0.0f, 0.0f);
    Vector up(0.0f, 1.0f, 0.0f);
    c.set_transform(view_transform(from, to, up));

    Canvas image = render(c, w);
    Color result = pixelAt(image, 5, 5);
    Color expected(0.38066f, 0.47583f, 0.2855f);

    EXPECT_NEAR(result.r, expected.r, 0.01f);
    EXPECT_NEAR(result.g, expected.g, 0.01f);
    EXPECT_NEAR(result.b, expected.b, 0.01f);
}
