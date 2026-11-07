export module rt.noise;

export namespace rt {

    /**
     * @brief Samples 3D Perlin noise (Ken Perlin's 2002 "improved noise").
     *
     * Returns a smooth, deterministic pseudo-random value that varies continuously with
     * the input: nearby points give similar values, distant points look unrelated. The
     * value is exactly 0 at every integer lattice point and stays within [-1, 1].
     * Used by perturbed patterns to jitter points organically.
     * Math reference: documentation/explanation/Patterns.md
     *
     * @param x The x coordinate of the sample.
     * @param y The y coordinate of the sample.
     * @param z The z coordinate of the sample.
     * @return float The noise value in [-1, 1].
     */
    [[nodiscard]] float perlin_noise(float x, float y, float z);

} // namespace rt
