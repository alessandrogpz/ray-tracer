#include <gtest/gtest.h>

import std;
import rt.colors;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.patterns;
import rt.sphere;

using namespace rt;

const Color BLACK(0.0f, 0.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);

TEST(StripePattern, CreatingAStripePattern)
{
    const Pattern pattern = stripe_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern.a, WHITE);
    EXPECT_EQ(pattern.b, BLACK);
}

TEST(StripePattern, AStripePatternIsConstantInY)
{
    const Pattern pattern = stripe_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 1.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 2.0f, 0.0f)), WHITE);
}

TEST(StripePattern, AStripePatternIsConstantInZ)
{
    const Pattern pattern = stripe_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 1.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 2.0f)), WHITE);
}

TEST(StripePattern, AStripePatternAlternatesInX)
{
    const Pattern pattern = stripe_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.9f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(1.0f, 0.0f, 0.0f)), BLACK);
    EXPECT_EQ(pattern_at(pattern, Point(-0.1f, 0.0f, 0.0f)), BLACK);
    EXPECT_EQ(pattern_at(pattern, Point(-1.0f, 0.0f, 0.0f)), BLACK);
    EXPECT_EQ(pattern_at(pattern, Point(-1.1f, 0.0f, 0.0f)), WHITE);
}

TEST(StripePattern, StripesWithAnObjectTransformation)
{
    Sphere object;
    object.set_transform(scale(2.0f, 2.0f, 2.0f));
    const Pattern pattern = stripe_pattern(WHITE, BLACK);

    const Color c = pattern_at_shape(pattern, object.get_transform_inverse(), Point(1.5f, 0.0f, 0.0f));

    EXPECT_EQ(c, WHITE);
}

TEST(StripePattern, StripesWithAPatternTransformation)
{
    const Sphere object;
    Pattern pattern = stripe_pattern(WHITE, BLACK);
    pattern.set_transform(scale(2.0f, 2.0f, 2.0f));

    const Color c = pattern_at_shape(pattern, object.get_transform_inverse(), Point(1.5f, 0.0f, 0.0f));

    EXPECT_EQ(c, WHITE);
}

TEST(StripePattern, StripesWithBothAnObjectAndAPatternTransformation)
{
    Sphere object;
    object.set_transform(scale(2.0f, 2.0f, 2.0f));
    Pattern pattern = stripe_pattern(WHITE, BLACK);
    pattern.set_transform(translation(0.5f, 0.0f, 0.0f));

    const Color c = pattern_at_shape(pattern, object.get_transform_inverse(), Point(2.5f, 0.0f, 0.0f));

    EXPECT_EQ(c, WHITE);
}

// ---------------------------------------------------
// Generalized Patterns

TEST(GeneralPattern, TheDefaultPatternTransformation)
{
    const Pattern pattern = test_pattern();

    EXPECT_EQ(pattern.get_transform(), identity());
}

TEST(GeneralPattern, AssigningATransformation)
{
    Pattern pattern = test_pattern();
    pattern.set_transform(translation(1.0f, 2.0f, 3.0f));

    EXPECT_EQ(pattern.get_transform(), translation(1.0f, 2.0f, 3.0f));
}

TEST(GeneralPattern, APatternWithAnObjectTransformation)
{
    Sphere shape;
    shape.set_transform(scale(2.0f, 2.0f, 2.0f));
    const Pattern pattern = test_pattern();

    const Color c = pattern_at_shape(pattern, shape.get_transform_inverse(), Point(2.0f, 3.0f, 4.0f));

    EXPECT_EQ(c, Color(1.0f, 1.5f, 2.0f));
}

TEST(GeneralPattern, APatternWithAPatternTransformation)
{
    const Sphere shape;
    Pattern pattern = test_pattern();
    pattern.set_transform(scale(2.0f, 2.0f, 2.0f));

    const Color c = pattern_at_shape(pattern, shape.get_transform_inverse(), Point(2.0f, 3.0f, 4.0f));

    EXPECT_EQ(c, Color(1.0f, 1.5f, 2.0f));
}

TEST(GeneralPattern, APatternWithBothAnObjectAndAPatternTransformation)
{
    Sphere shape;
    shape.set_transform(scale(2.0f, 2.0f, 2.0f));
    Pattern pattern = test_pattern();
    pattern.set_transform(translation(0.5f, 1.0f, 1.5f));

    const Color c = pattern_at_shape(pattern, shape.get_transform_inverse(), Point(2.5f, 3.0f, 3.5f));

    EXPECT_EQ(c, Color(0.75f, 0.5f, 0.25f));
}

// ---------------------------------------------------
// Gradient, Ring and Checker Patterns

TEST(GradientPattern, AGradientLinearlyInterpolatesBetweenColors)
{
    const Pattern pattern = gradient_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.25f, 0.0f, 0.0f)), Color(0.75f, 0.75f, 0.75f));
    EXPECT_EQ(pattern_at(pattern, Point(0.5f, 0.0f, 0.0f)), Color(0.5f, 0.5f, 0.5f));
    EXPECT_EQ(pattern_at(pattern, Point(0.75f, 0.0f, 0.0f)), Color(0.25f, 0.25f, 0.25f));
}

TEST(RingPattern, ARingShouldExtendInBothXAndZ)
{
    const Pattern pattern = ring_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(1.0f, 0.0f, 0.0f)), BLACK);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 1.0f)), BLACK);
    // 0.708 = just slightly more than √2/2
    EXPECT_EQ(pattern_at(pattern, Point(0.708f, 0.0f, 0.708f)), BLACK);
}

TEST(CheckersPattern, CheckersShouldRepeatInX)
{
    const Pattern pattern = checkers_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.99f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(1.01f, 0.0f, 0.0f)), BLACK);
}

TEST(CheckersPattern, CheckersShouldRepeatInY)
{
    const Pattern pattern = checkers_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.99f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 1.01f, 0.0f)), BLACK);
}

TEST(CheckersPattern, CheckersShouldRepeatInZ)
{
    const Pattern pattern = checkers_pattern(WHITE, BLACK);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.99f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 1.01f)), BLACK);
}

// ---------------------------------------------------
// Solid and Nested Patterns

TEST(SolidPattern, ASolidPatternIsTheSameColorEverywhere)
{
    const Color teal(0.1f, 0.7f, 0.6f);
    const Pattern pattern = solid_pattern(teal);

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), teal);
    EXPECT_EQ(pattern_at(pattern, Point(1.5f, -2.0f, 7.25f)), teal);
    EXPECT_EQ(pattern_at(pattern, Point(-3.3f, 4.4f, -5.5f)), teal);
}

TEST(NestedPattern, StripesOfSolidPatternsMatchPlainStripes)
{
    const Pattern plain = stripe_pattern(WHITE, BLACK);
    const Pattern nested = stripe_pattern(solid_pattern(WHITE), solid_pattern(BLACK));

    for (const float x : {-1.1f, -1.0f, -0.1f, 0.0f, 0.9f, 1.0f, 2.5f})
    {
        const Point p(x, 0.0f, 0.0f);
        EXPECT_EQ(pattern_at(nested, p), pattern_at(plain, p)) << "at x = " << x;
    }
}

TEST(NestedPattern, EachChildIsEvaluatedInItsOwnSpace)
{
    // Checkers whose white squares hold stripes along x and whose black squares hold
    // stripes rotated to run along z (the book's nested-pattern example)
    const Color red(1.0f, 0.0f, 0.0f);
    const Color green(0.0f, 1.0f, 0.0f);
    const Pattern along_x = stripe_pattern(WHITE, red);
    Pattern along_z = stripe_pattern(BLACK, green);
    // Rotated a quarter turn (stripes now vary along z) and 4x narrower: child x = -4z
    along_z.set_transform(rotation_y(std::numbers::pi_v<float> / 2.0f) * scale(0.25f, 1.0f, 0.25f));
    const Pattern pattern = checkers_pattern(along_x, along_z);

    // First checker square (even): stripes along x, so x = 0.5 is the first stripe
    EXPECT_EQ(pattern_at(pattern, Point(0.5f, 0.0f, 0.5f)), WHITE);

    // Second checker square (x in [1, 2), z in [0, 1)): the rotated stripes change with z...
    EXPECT_EQ(pattern_at(pattern, Point(1.5f, 0.0f, 0.2f)), green);    // child x = -0.8
    EXPECT_EQ(pattern_at(pattern, Point(1.5f, 0.0f, 0.3f)), BLACK);    // child x = -1.2
    EXPECT_EQ(pattern_at(pattern, Point(1.5f, 0.0f, 0.6f)), green);    // child x = -2.4

    // ...but not with x
    EXPECT_EQ(pattern_at(pattern, Point(1.2f, 0.0f, 0.3f)), BLACK);
    EXPECT_EQ(pattern_at(pattern, Point(1.8f, 0.0f, 0.3f)), BLACK);
}

TEST(NestedPattern, AGradientBlendsItsChildPatterns)
{
    const Pattern pattern = gradient_pattern(solid_pattern(WHITE), solid_pattern(BLACK));

    EXPECT_EQ(pattern_at(pattern, Point(0.0f, 0.0f, 0.0f)), WHITE);
    EXPECT_EQ(pattern_at(pattern, Point(0.5f, 0.0f, 0.0f)), Color(0.5f, 0.5f, 0.5f));
    EXPECT_EQ(pattern_at(pattern, Point(0.75f, 0.0f, 0.0f)), Color(0.25f, 0.25f, 0.25f));
}

TEST(NestedPattern, NestedPatternsCanBeCompared)
{
    const Pattern a = checkers_pattern(stripe_pattern(WHITE, BLACK), solid_pattern(WHITE));
    const Pattern b = checkers_pattern(stripe_pattern(WHITE, BLACK), solid_pattern(WHITE));
    const Pattern c = checkers_pattern(stripe_pattern(WHITE, BLACK), solid_pattern(BLACK));

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

// ---------------------------------------------------
// Perturbed Patterns

TEST(PerturbedPattern, NoPerturbationLeavesThePatternUnchanged)
{
    const Pattern stripes = stripe_pattern(WHITE, BLACK);
    const Pattern pattern = perturbed_pattern(stripes, 0.0f);

    for (int i = 0; i < 50; ++i)
    {
        const Point p(static_cast<float>(i) * 0.173f - 4.0f, static_cast<float>(i) * 0.05f, 0.3f);
        EXPECT_EQ(pattern_at(pattern, p), pattern_at(stripes, p));
    }
}

TEST(PerturbedPattern, PerturbingASolidPatternChangesNothing)
{
    const Color amber(1.0f, 0.7f, 0.15f);
    const Pattern pattern = perturbed_pattern(solid_pattern(amber), 0.5f);

    EXPECT_EQ(pattern_at(pattern, Point(0.3f, 0.6f, 0.9f)), amber);
    EXPECT_EQ(pattern_at(pattern, Point(-2.1f, 5.4f, 1.7f)), amber);
}

TEST(PerturbedPattern, PerturbationMovesTheStripeBoundaries)
{
    const Pattern stripes = stripe_pattern(WHITE, BLACK);
    const Pattern pattern = perturbed_pattern(stripes, 0.3f);

    // Sample points close to stripe boundaries; jitter must flip some of them
    int changed = 0;
    for (int i = 0; i < 200; ++i)
    {
        const float x = std::floor(static_cast<float>(i) * 0.37f) + 0.05f;
        const Point p(x, static_cast<float>(i) * 0.211f, static_cast<float>(i) * 0.093f);
        if (!(pattern_at(pattern, p) == pattern_at(stripes, p)))
            ++changed;
    }

    EXPECT_GT(changed, 0);
}

TEST(PerturbedPattern, ThePerturbedPatternKeepsItsOwnTransform)
{
    Pattern inner = test_pattern();
    inner.set_transform(scale(2.0f, 2.0f, 2.0f));
    const Pattern pattern = perturbed_pattern(inner, 0.0f);

    EXPECT_EQ(pattern_at(pattern, Point(2.0f, 3.0f, 4.0f)), Color(1.0f, 1.5f, 2.0f));
}
