export module rt.utils;

export namespace rt {
    /// Absolute tolerance for comparing floats near zero, and the offset used to nudge points off surfaces.
    constexpr float EPSILON = 0.0001f;

    /// Relative tolerance for comparing large floats: about 84 ULPs of single precision (machine epsilon ≈ 1.19e-7).
    constexpr float RELATIVE_EPSILON = 0.00001f;

    /**
     * @brief Compares two floats for approximate equality with a combined absolute and relative tolerance.
     *
     * Two values are equal if they differ by less than EPSILON (absolute, so comparisons against
     * zero work) or by at most RELATIVE_EPSILON times the larger magnitude (relative, so large
     * values are not held to a tolerance finer than float precision can represent). The two
     * tolerances meet at magnitude 10. Identical values, including equal infinities, are always equal.
     * Reference: documentation/references/CoreMath.md
     *
     * @param a The first value.
     * @param b The second value.
     * @return true If a and b are equal within tolerance.
     */
    [[nodiscard]]
    bool equal(float a, float b);
} // namespace rt
