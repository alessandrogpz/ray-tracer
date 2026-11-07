#include <gtest/gtest.h>

import std;

import rt.utils;
import rt.tuple;
import rt.colors;
import rt.matrix;
import rt.transformations;
import rt.ray;
import rt.sphere;
import rt.plane;
import rt.intersection;
import rt.lights;
import rt.world;

using namespace rt;

// Floating-point stability tests
// Reference: documentation/references/CoreMath.md

namespace
{
    constexpr float PI = std::numbers::pi_v<float>;
    constexpr int SAMPLES = 500;

    // Deterministic random numbers: std::mt19937 produces the same sequence on every platform,
    // and mapping its output by hand (instead of std::uniform_real_distribution, whose algorithm
    // differs between standard libraries) keeps every failure reproducible.
    class Random
    {
    public:
        explicit Random(std::uint32_t seed) : engine(seed) {}

        float uniform(float low, float high)
        {
            const double unit = static_cast<double>(engine()) / 4294967296.0;  // [0, 1)
            return low + static_cast<float>(unit) * (high - low);
        }

        Vector unitVector()
        {
            // Uniform direction on the sphere from a random height and angle
            const float z = uniform(-1.0f, 1.0f);
            const float angle = uniform(0.0f, 2.0f * PI);
            const float r = std::sqrt(1.0f - z * z);
            return {r * std::cos(angle), r * std::sin(angle), z};
        }

        // Translation, rotation about all three axes, and a non-uniform scale from `min_scale` to `max_scale`
        Matrix<4> transform(float max_translation, float min_scale, float max_scale)
        {
            return translation(uniform(-max_translation, max_translation),
                               uniform(-max_translation, max_translation),
                               uniform(-max_translation, max_translation)) *
                   rotation_x(uniform(0.0f, 2.0f * PI)) *
                   rotation_y(uniform(0.0f, 2.0f * PI)) *
                   rotation_z(uniform(0.0f, 2.0f * PI)) *
                   scale(uniform(min_scale, max_scale), uniform(min_scale, max_scale), uniform(min_scale, max_scale));
        }

    private:
        std::mt19937 engine;
    };

    // A Point at the tip of a direction vector (a Point needs w = 1, so it cannot be built from a Vector directly)
    Point pointAt(Vector v)
    {
        return {v.x, v.y, v.z};
    }

    bool isFinite(Color c)
    {
        return std::isfinite(c.r) && std::isfinite(c.g) && std::isfinite(c.b);
    }
}

// ---------------------------------------------------
// Comparing Floats: absolute + relative tolerance

TEST(FloatComparison, NearZeroTheAbsoluteToleranceApplies)
{
    EXPECT_TRUE(equal(0.00005f, 0.0f));
    EXPECT_TRUE(equal(1e-9f, 0.0f));         // a purely relative tolerance would reject this
    EXPECT_FALSE(equal(0.0002f, 0.0f));
}

TEST(FloatComparison, LargeValuesUseARelativeTolerance)
{
    // At 100,000 neighbouring floats are ~0.008 apart, so an absolute 0.0001 could never match
    const float big = 100000.0f;
    EXPECT_TRUE(equal(big, std::nextafter(big, 2.0f * big)));
    EXPECT_TRUE(equal(big, big + 0.5f));    // relative difference 5e-6
    EXPECT_FALSE(equal(big, big + 10.0f));  // relative difference 1e-4
}

TEST(FloatComparison, InfinitiesOnlyEqualThemselves)
{
    const float inf = std::numeric_limits<float>::infinity();
    EXPECT_TRUE(equal(inf, inf));
    EXPECT_TRUE(equal(-inf, -inf));
    EXPECT_FALSE(equal(inf, -inf));
    EXPECT_FALSE(equal(inf, 1e30f));
}

TEST(FloatComparison, NaNIsNeverEqual)
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    EXPECT_FALSE(equal(nan, nan));
    EXPECT_FALSE(equal(nan, 0.0f));
}

// ---------------------------------------------------
// Matrix Invertibility at Extreme Scales

TEST(MatrixStability, TinyUniformScalesAreInvertible)
{
    for (const float s : {0.04f, 0.01f, 0.001f})
    {
        const Matrix<4> m = scale(s, s, s);

        ASSERT_TRUE(m.isInvertible()) << "scale " << s << " (determinant " << m.determinant() << ")";
        EXPECT_EQ(m * m.inverse(), identity()) << "scale " << s;
    }
}

TEST(MatrixStability, LargeUniformScalesAreInvertible)
{
    for (const float s : {100.0f, 1000.0f})
    {
        const Matrix<4> m = scale(s, s, s);

        ASSERT_TRUE(m.isInvertible()) << "scale " << s;
        EXPECT_EQ(m * m.inverse(), identity()) << "scale " << s;
    }
}

TEST(MatrixStability, ANumericallySingularMatrixIsRejected)
{
    // The third row is the sum of the first two, computed in float, so the determinant is
    // only zero up to rounding; it must still be recognised as singular
    Matrix<4> m;
    const std::array<float, 4> r0 = {0.1f, 0.2f, 0.3f, 0.4f};
    const std::array<float, 4> r1 = {0.7f, 0.11f, 0.13f, 0.17f};
    for (std::size_t c = 0; c < 4; ++c)
    {
        m(0, c) = r0[c];
        m(1, c) = r1[c];
        m(2, c) = r0[c] + r1[c];
        m(3, c) = static_cast<float>(c + 1);
    }

    EXPECT_FALSE(m.isInvertible());
}

TEST(MatrixStability, GrazingRaysHitPlanesAtAnyScale)
{
    // A plane's object-space ray direction is not normalized: scaling the plane by s scales it
    // by 1/s. An absolute |D_y| < EPSILON test would therefore treat a 3° ray as parallel to a
    // plane scaled by 1000. The test is relative to the direction's length, so it must not be.
    for (const float s : {0.001f, 1.0f, 1000.0f})
    {
        Plane p;
        p.set_transform(scale(s, s, s));

        for (const float degrees : {30.0f, 3.0f, 0.5f})
        {
            const float angle = degrees * PI / 180.0f;
            const Ray r(Point(0.0f, 1.0f, 0.0f), Vector(std::cos(angle), -std::sin(angle), 0.0f));
            const std::vector<Intersection> xs = intersect(p, r);

            // Starting 1 unit above the plane, the ray reaches it after 1 / sin(angle)
            ASSERT_EQ(xs.size(), 1) << "scale " << s << ", " << degrees << " degrees";
            EXPECT_TRUE(equal(xs[0].t, 1.0f / std::sin(angle))) << "scale " << s << ", " << degrees << " degrees";
        }

        // A truly parallel ray still misses
        EXPECT_TRUE(intersect(p, Ray(Point(0.0f, 1.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f))).empty()) << "scale " << s;
    }
}

TEST(MatrixStability, ASmallSphereCanBeTransformedAndHit)
{
    Sphere marble;
    ASSERT_NO_THROW(marble.set_transform(translation(0.0f, 0.0f, 1.0f) * scale(0.02f, 0.02f, 0.02f)));

    const Ray r(Point(0.0f, 0.0f, -5.0f), Vector(0.0f, 0.0f, 1.0f));
    const std::vector<Intersection> xs = intersect(marble, r);

    ASSERT_EQ(xs.size(), 2);
    EXPECT_TRUE(equal(xs[0].t, 5.98f));
    EXPECT_TRUE(equal(xs[1].t, 6.02f));
}

// ---------------------------------------------------
// Invariants over Many Random Inputs

TEST(InvariantStability, RandomTransformsTimesTheirInverseGiveTheIdentity)
{
    Random random(1);
    for (int i = 0; i < SAMPLES; ++i)
    {
        const Matrix<4> m = random.transform(100.0f, 0.05f, 5.0f);
        const Matrix<4> product = m * m.inverse();

        for (std::size_t r = 0; r < 4; ++r)
            for (std::size_t c = 0; c < 4; ++c)
                // Worst error measured over 20,000 samples: 3e-5
                ASSERT_NEAR(product(r, c), r == c ? 1.0f : 0.0f, 0.0001f) << "sample " << i << " entry (" << r << ", " << c << ")";
    }
}

TEST(InvariantStability, HitPointsLieOnTheSurface)
{
    Random random(2);
    for (int i = 0; i < SAMPLES; ++i)
    {
        Sphere s;
        s.set_transform(random.transform(50.0f, 0.1f, 3.0f));

        // Aim from far away at the sphere's centre, from a random direction
        const Point centre = s.get_transform() * Point(0.0f, 0.0f, 0.0f);
        const Vector direction = random.unitVector();
        const Ray r(centre - direction * 20.0f, direction);

        // The ray starts far away relative to the (possibly tiny) sphere, up to ~700 units in its
        // object space, and the quadratic formula loses digits subtracting nearly equal large
        // numbers (catastrophic cancellation). Worst error measured over 20,000 samples: 5.3e-3
        for (const Intersection& x : intersect(s, r))
        {
            const Point local = s.get_transform_inverse() * position(r, x.t);
            const float distance_from_centre = getVectorMagnitude(local - Point(0.0f, 0.0f, 0.0f));
            ASSERT_NEAR(distance_from_centre, 1.0f, 0.01f) << "sample " << i;
        }
    }
}

TEST(InvariantStability, NormalsHaveUnitLength)
{
    Random random(3);
    for (int i = 0; i < SAMPLES; ++i)
    {
        Sphere s;
        s.set_transform(random.transform(50.0f, 0.05f, 5.0f));

        const Point on_surface = s.get_transform() * pointAt(random.unitVector());
        const Vector n = normalAt(s, on_surface);

        ASSERT_NEAR(getVectorMagnitude(n), 1.0f, 0.00001f) << "sample " << i;
        ASSERT_FLOAT_EQ(n.w, 0.0f) << "sample " << i;
    }
}

TEST(InvariantStability, ReflectionPreservesLength)
{
    Random random(4);
    for (int i = 0; i < SAMPLES; ++i)
    {
        const Vector v = random.unitVector() * random.uniform(0.01f, 100.0f);
        const Vector n = random.unitVector();

        ASSERT_TRUE(equal(getVectorMagnitude(reflect(v, n)), getVectorMagnitude(v))) << "sample " << i;
    }
}

TEST(InvariantStability, MatchingRefractiveIndicesDoNotBendLight)
{
    // A fully transparent sphere with the refractive index of the surrounding vacuum, and no
    // surface color of its own, must be invisible: every ray sees the wall behind it unchanged
    World without_sphere;
    without_sphere.light = PointLight(Point(-5.0f, 5.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    Plane wall;
    wall.set_transform(translation(0.0f, 0.0f, 5.0f) * rotation_x(PI / 2.0f));
    wall.material.color = Color(0.8f, 0.5f, 0.3f);
    without_sphere.add_plane(wall);

    World with_sphere = without_sphere;
    Sphere ghost = glass_sphere();
    ghost.material.refractive_index = 1.0f;
    ghost.material.ambient = 0.0f;
    ghost.material.diffuse = 0.0f;
    ghost.material.specular = 0.0f;
    ghost.material.casts_shadow = false;
    with_sphere.add_sphere(ghost);

    Random random(5);
    for (int i = 0; i < SAMPLES; ++i)
    {
        // Rays from in front of the sphere, through a random point on its disc
        const Point target(random.uniform(-0.9f, 0.9f), random.uniform(-0.9f, 0.9f), 0.0f);
        const Point origin(random.uniform(-0.5f, 0.5f), random.uniform(-0.5f, 0.5f), -5.0f);
        const Ray r(origin, normalizeVector(target - origin));

        const Color seen = color_at(with_sphere, r);
        const Color expected = color_at(without_sphere, r);
        ASSERT_NEAR(seen.r, expected.r, 0.001f) << "sample " << i;
        ASSERT_NEAR(seen.g, expected.g, 0.001f) << "sample " << i;
        ASSERT_NEAR(seen.b, expected.b, 0.001f) << "sample " << i;
    }
}

TEST(InvariantStability, SchlickReflectanceStaysBetweenZeroAndOne)
{
    World w;
    w.add_sphere(glass_sphere());

    Random random(6);
    for (int i = 0; i < SAMPLES; ++i)
    {
        // Half the rays start outside the sphere, half inside (where total internal reflection happens)
        const bool inside = i % 2 == 0;
        const Point origin = inside ? pointAt(random.unitVector() * random.uniform(0.0f, 0.9f))
                                    : pointAt(random.unitVector() * random.uniform(2.0f, 5.0f));
        const Ray r(origin, inside ? random.unitVector() : normalizeVector(pointAt(random.unitVector() * 0.9f) - origin));

        const std::vector<Intersection> xs = intersect_world(w, r);
        const std::optional<Intersection> h = hit(xs);
        if (!h)
            continue;

        const float reflectance = schlick(prepare_computation(*h, r, w, xs));
        ASSERT_GE(reflectance, 0.0f) << "sample " << i;
        ASSERT_LE(reflectance, 1.0f) << "sample " << i;
    }
}

TEST(InvariantStability, RenderedColorsAreNeverNaNOrInfinite)
{
    // A scene with every kind of secondary ray: reflection, refraction, Fresnel, and shadows
    World w = default_world();
    Plane floor_plane;
    floor_plane.set_transform(translation(0.0f, -1.0f, 0.0f));
    floor_plane.material.reflective = 0.5f;
    w.add_plane(floor_plane);
    Sphere glass = glass_sphere();
    glass.set_transform(translation(2.0f, 0.0f, 0.0f));
    glass.material.reflective = 0.9f;
    w.add_sphere(glass);

    Random random(7);
    for (int i = 0; i < SAMPLES; ++i)
    {
        const Point origin(random.uniform(-6.0f, 6.0f), random.uniform(-0.5f, 6.0f), random.uniform(-6.0f, -2.0f));
        const Ray r(origin, random.unitVector());

        ASSERT_TRUE(isFinite(color_at(w, r))) << "sample " << i;
    }
}

// ---------------------------------------------------
// Precision Far from the Origin

TEST(MagnitudeStability, OverPointClearsTheSurfaceAtAnyDistance)
{
    // Float spacing grows with distance (about 0.00006 at 1,000 units, 0.001 at 10,000 and
    // 0.008 at 100,000). A fixed EPSILON offset would fall below it and cause shadow acne, so
    // the offset grows with the coordinates beyond 100 units. No hit may shadow itself, anywhere.
    Random random(8);
    for (const float distance : {0.0f, 100.0f, 1000.0f, 10000.0f, 100000.0f})
    {
        World w;
        Sphere s;
        s.set_transform(translation(distance, 0.0f, 0.0f));
        w.add_sphere(s);

        for (int i = 0; i < SAMPLES; ++i)
        {
            const Vector direction = random.unitVector();
            const Ray r(Point(distance, 0.0f, 0.0f) - direction * 5.0f, direction);
            const std::vector<Intersection> xs = intersect_world(w, r);
            const std::optional<Intersection> h = hit(xs);
            if (!h)
                continue;

            // A ray leaving over_point along the normal must not hit the surface it started on
            const Comp c = prepare_computation(*h, r, w, xs);
            for (const Intersection& x : intersect_world(w, Ray(c.over_point, c.normal_v)))
                ASSERT_LE(x.t, EPSILON) << "distance " << distance << ", sample " << i;
        }
    }
}
