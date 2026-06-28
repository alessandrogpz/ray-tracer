export module rt.utils;

export namespace rt {
    constexpr float EPSILON = 0.0001f;

    [[nodiscard]]
    bool equal(float a, float b);
} // namespace rt
