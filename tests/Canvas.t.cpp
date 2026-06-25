#include <gtest/gtest.h>

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.canvas;
import rt.matrix;
import rt.transformations;

using namespace rt;

// ---------------------------------------------------
// Canvas Creation

TEST(CanvasTest, CreatingCanvas) {
    Canvas c(10, 20);
    EXPECT_EQ(c.width, 10);
    EXPECT_EQ(c.height, 20);
    
    // Check that every pixel is black
    for (const auto& p : c.pixels) {
        EXPECT_TRUE(equal(p.r, 0));
        EXPECT_TRUE(equal(p.g, 0));
        EXPECT_TRUE(equal(p.b, 0));
    }
}

// ---------------------------------------------------
// Write Pixel

TEST(CanvasTest, WritingPixels) {
    Canvas c(10, 20);
    Color red(1, 0, 0);
    writePixel(c, 2, 3, red);
    
    Color result = pixelAt(c, 2, 3);
    EXPECT_TRUE(equal(result.r, 1));
    EXPECT_TRUE(equal(result.g, 0));
    EXPECT_TRUE(equal(result.b, 0));
}

// ---------------------------------------------------
// Canvas to PPM

TEST(CanvasToPPM, PPMHeader) {
    Canvas c(5, 3);

    std::string ppm = canvasToPPM(c);
    std::stringstream ss(ppm);
    std::string line;

    // Line 1: Magic Number
    std::getline(ss, line);
    EXPECT_EQ(line, "P3");

    // Line 2: Dimensions (Width Height)
    std::getline(ss, line);
    EXPECT_EQ(line, "5 3");

    // Line 3: Maximum Color Value
    std::getline(ss, line);
    EXPECT_EQ(line, "255");
}

TEST(CanvasToPPM, PPMPixelData) {
    Canvas c(5, 3);

    Color c1(1.5, 0.0, 0.0);
    Color c2(0.0, 0.5, 0.0);
    Color c3(-0.5, 0.0, 1.0);

    writePixel(c, 0, 0, c1);
    writePixel(c, 2, 1, c2);
    writePixel(c, 4, 2, c3);

    std::string ppm = canvasToPPM(c);
    std::stringstream ss(ppm);
    std::string line;

    // Get past PPM header
    for (int i = 0; i < 3; i++)
        std::getline(ss, line);

    // Test Pixel Data
    std::getline(ss, line);
    EXPECT_EQ(line, "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");

    std::getline(ss, line);
    EXPECT_EQ(line, "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");

    std::getline(ss, line);
    EXPECT_EQ(line, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
}

TEST(CanvasToPPM, PPMLongPixelDataLines) {
    Color c1(1.0, 0.8, 0.6);
    
    Canvas c(10, 2, c1);

    std::string ppm = canvasToPPM(c);
    std::stringstream ss(ppm);
    std::string line;

    // Get past PPM header
    for (int i = 0; i < 3; i++)
        std::getline(ss, line);

    // Test Long Pixel Data
    std::getline(ss, line);
    EXPECT_EQ(line, "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");

    std::getline(ss, line);
    EXPECT_EQ(line, "153 255 204 153 255 204 153 255 204 153 255 204 153");

    std::getline(ss, line);
    EXPECT_EQ(line, "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");

    std::getline(ss, line);
    EXPECT_EQ(line, "153 255 204 153 255 204 153 255 204 153 255 204 153");
}

TEST(CanvasToPPM, PPMNewlineTerminated) {
    Canvas c(5, 3);
    std::string ppm = canvasToPPM(c);

    ASSERT_FALSE(ppm.empty());
    
    // Checks that the very last byte of the string is a newline.
    EXPECT_EQ(ppm.back(), '\n');
    
    // Ensure the character before it ISN'T a newline to prevent the "double newline" bug.
    if (ppm.size() > 1) {
        EXPECT_NE(ppm[ppm.size() - 2], '\n');
    }
}