export module rt.patterns;

import std;

import rt.colors;
import rt.tuple;
import rt.matrix;
import rt.transformations;

export namespace rt {

    /**
     * @brief Identifies which color function a Pattern evaluates.
     *
     * Patterns are flat value types (no inheritance), so the concrete behaviour is
     * selected with a switch on this enum, mirroring how ShapeType dispatches shapes.
     */
    enum class PatternType : std::uint8_t {
        Solid,      ///< A single color everywhere; the leaf of nested patterns
        Stripe,
        Gradient,
        Ring,
        Checkers,
        Perturbed,  ///< Jitters each point with Perlin noise, then evaluates its one child pattern
        Test        ///< Returns the pattern-space point as a color; used to verify transforms in tests
    };

    /**
     * @brief A procedural texture that maps a 3D point to a Color.
     *
     * Every pattern alternates or blends between two colors `a` and `b`, and owns a
     * transformation from object space to pattern space, so a pattern can be scaled,
     * rotated, or shifted independently of the shape it decorates.
     *
     * Patterns can be nested: when `children` holds two patterns, they replace `a` and `b`
     * (each evaluated in its own transformed space), e.g. checkers whose squares contain
     * stripes. A perturbed pattern holds exactly one child, which it evaluates at jittered points.
     * Math reference: documentation/explanation/Patterns.md
     */
    struct Pattern {
        PatternType type{PatternType::Stripe};
        Color a{};
        Color b{};
        Matrix<4> transform{identity()};
        Matrix<4> transform_inverse{identity()};
        std::vector<Pattern> children{};    ///< Nested patterns used instead of a/b (two), or the perturbed pattern (one)
        float perturbation{0.0f};           ///< Perturbed only: how far, in pattern units, points are jittered

        void set_transform(const Matrix<4>& t);
        [[nodiscard]] const Matrix<4>& get_transform() const;
        [[nodiscard]] const Matrix<4>& get_transform_inverse() const;

        [[nodiscard]] bool operator==(const Pattern& other) const = default;
    };

    // Factory Utilities
    [[nodiscard]] Pattern solid_pattern(Color color);
    [[nodiscard]] Pattern stripe_pattern(Color a, Color b);
    [[nodiscard]] Pattern gradient_pattern(Color a, Color b);
    [[nodiscard]] Pattern ring_pattern(Color a, Color b);
    [[nodiscard]] Pattern checkers_pattern(Color a, Color b);
    [[nodiscard]] Pattern test_pattern();

    // Nested Factory Utilities: sub-patterns take the place of the two colors
    [[nodiscard]] Pattern stripe_pattern(Pattern a, Pattern b);
    [[nodiscard]] Pattern gradient_pattern(Pattern a, Pattern b);
    [[nodiscard]] Pattern ring_pattern(Pattern a, Pattern b);
    [[nodiscard]] Pattern checkers_pattern(Pattern a, Pattern b);

    /**
     * @brief Wraps a pattern so it is evaluated at points jittered by 3D Perlin noise.
     *
     * Each point is displaced by up to `perturbation` units along every axis before the
     * inner pattern sees it, so straight stripes become wavy, organic, marble-like bands.
     * Math reference: documentation/explanation/Patterns.md
     *
     * @param pattern The pattern to perturb (it keeps its own transform).
     * @param perturbation The maximum jitter distance, in pattern units (around 0.1 to 0.3 looks natural).
     * @return Pattern The perturbed pattern.
     */
    [[nodiscard]] Pattern perturbed_pattern(Pattern pattern, float perturbation = 0.2f);

    /**
     * @brief Evaluates the pattern's color function at a point already in pattern space.
     *
     * - Stripe:   `a` when floor(x) is even, `b` otherwise.
     * - Gradient: linear blend from `a` to `b` across the fractional part of x.
     * - Ring:     `a` when floor(sqrt(x² + z²)) is even, `b` otherwise.
     * - Checkers: `a` when floor(x) + floor(y) + floor(z) is even, `b` otherwise.
     * - Solid:    always `a`.
     * - Perturbed: the child pattern, evaluated at the point jittered by Perlin noise.
     * - Test:     the point's (x, y, z) as (r, g, b).
     * For nested patterns, `a` and `b` stand for the two child patterns, each evaluated
     * at the point transformed by that child's own inverse transform.
     * Math reference: documentation/explanation/Patterns.md
     *
     * @param pattern The Pattern to evaluate.
     * @param pattern_point The Point expressed in the pattern's own coordinate space.
     * @return Color The color of the pattern at that point.
     */
    [[nodiscard]] Color pattern_at(const Pattern& pattern, Point pattern_point);

    /**
     * @brief Evaluates a pattern at a world-space point on a given shape.
     *
     * Converts the point from world space to object space (with the shape's inverse
     * transform), then from object space to pattern space (with the pattern's inverse
     * transform), so the pattern moves, scales, and rotates together with its shape.
     * Math reference: documentation/explanation/Patterns.md
     *
     * @param pattern The Pattern to evaluate.
     * @param object_inverse The inverse transform of the shape the pattern is applied to (world to object space).
     * @param world_point The Point on the shape's surface, in world space.
     * @return Color The color of the pattern at that point.
     */
    [[nodiscard]] Color pattern_at_shape(const Pattern& pattern, const Matrix<4>& object_inverse, Point world_point);

} // namespace rt
