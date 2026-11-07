module rt.noise;

import std;

namespace rt {

    namespace {
        // The permutation table: 0..255 shuffled with a fixed seed, then repeated once so
        // lookups like table[table[i] + j] never need wrapping. Any permutation works for
        // Perlin noise; a fixed seed keeps renders identical from run to run.
        constexpr std::array<int, 512> makePermutation() {
            std::array<int, 256> p{};
            for (int i = 0; i < 256; ++i) {
                p[i] = i;
            }

            // Fisher-Yates shuffle driven by a small linear congruential generator
            std::uint32_t state = 2026u;
            for (int i = 255; i > 0; --i) {
                state = state * 1664525u + 1013904223u;
                const int j = static_cast<int>((state >> 8) % static_cast<std::uint32_t>(i + 1));
                const int tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }

            std::array<int, 512> table{};
            for (int i = 0; i < 512; ++i) {
                table[i] = p[i % 256];
            }
            return table;
        }

        constexpr std::array<int, 512> PERMUTATION = makePermutation();

        // Smoothstep-like easing 6t⁵ - 15t⁴ + 10t³: zero first and second derivatives at 0 and 1
        float fade(float t) {
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }

        float lerp(float t, float a, float b) {
            return a + t * (b - a);
        }

        // Dot product of the offset (x, y, z) with one of 12 cube-edge gradient directions picked by the hash
        float grad(int hash, float x, float y, float z) {
            const int h = hash & 15;
            const float u = h < 8 ? x : y;
            const float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }
    } // namespace

    float perlin_noise(float x, float y, float z) {
        const std::array<int, 512>& p = PERMUTATION;

        // The unit cube containing the point, and the point's position inside it
        const float fx = std::floor(x);
        const float fy = std::floor(y);
        const float fz = std::floor(z);
        const int xi = static_cast<int>(fx) & 255;
        const int yi = static_cast<int>(fy) & 255;
        const int zi = static_cast<int>(fz) & 255;
        x -= fx;
        y -= fy;
        z -= fz;

        const float u = fade(x);
        const float v = fade(y);
        const float w = fade(z);

        // Hash the 8 cube corners
        const int a  = p[xi] + yi;
        const int aa = p[a] + zi;
        const int ab = p[a + 1] + zi;
        const int b  = p[xi + 1] + yi;
        const int ba = p[b] + zi;
        const int bb = p[b + 1] + zi;

        // Blend the corner contributions
        return lerp(w,
            lerp(v,
                lerp(u, grad(p[aa], x, y, z),             grad(p[ba], x - 1.0f, y, z)),
                lerp(u, grad(p[ab], x, y - 1.0f, z),      grad(p[bb], x - 1.0f, y - 1.0f, z))),
            lerp(v,
                lerp(u, grad(p[aa + 1], x, y, z - 1.0f),        grad(p[ba + 1], x - 1.0f, y, z - 1.0f)),
                lerp(u, grad(p[ab + 1], x, y - 1.0f, z - 1.0f), grad(p[bb + 1], x - 1.0f, y - 1.0f, z - 1.0f))));
    }

} // namespace rt
