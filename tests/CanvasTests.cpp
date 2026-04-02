#include <gtest/gtest.h>
#include "../includes/Tuple.hpp"
#include "../includes/Canvas.hpp"

// ---------------------------------------------------
// Canvas Creation

TEST(CanvasTest, CreatingCanvas) {
    canvas c(10, 20);
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
    canvas c(10, 20);
    color red(1, 0, 0);
    writePixel(c, 2, 3, red);
    
    color result = pixelAt(c, 2, 3);
    EXPECT_TRUE(equal(result.r, 1));
    EXPECT_TRUE(equal(result.g, 0));
    EXPECT_TRUE(equal(result.b, 0));
}