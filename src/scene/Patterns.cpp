module rt.patterns;

import std;
import rt.colors;
import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.noise;

namespace rt
{
    namespace {
        // True when the whole unit containing `value` is even. floor (not truncation) keeps
        // negative values right: floor(-0.1) = -1, which is odd.
        bool isEven(float value) {
            return static_cast<long long>(std::floor(value)) % 2 == 0;
        }

        // Evaluates a child pattern at a point given in its parent's pattern space
        Color childAt(const Pattern& child, Point parent_point) {
            return pattern_at(child, child.get_transform_inverse() * parent_point);
        }

        // The color for slot 0 (`a`) or 1 (`b`): a nested child pattern if present, otherwise the plain color
        Color slotColor(const Pattern& pattern, std::size_t slot, Point point) {
            if (pattern.children.size() == 2) {
                return childAt(pattern.children[slot], point);
            }
            return slot == 0 ? pattern.a : pattern.b;
        }

        Pattern nested(PatternType type, Pattern a, Pattern b) {
            Pattern pattern{.type = type};
            pattern.children.reserve(2);
            pattern.children.push_back(std::move(a));
            pattern.children.push_back(std::move(b));
            return pattern;
        }

        // Offsets that decorrelate the three noise samples used for the x, y and z jitter
        constexpr float NOISE_OFFSET_Y = 31.416f;
        constexpr float NOISE_OFFSET_Z = 73.172f;
    } // namespace

    void Pattern::set_transform(const Matrix<4>& t) {
        transform = t;
        transform_inverse = t.inverse();
    }

    const Matrix<4>& Pattern::get_transform() const {
        return transform;
    }

    const Matrix<4>& Pattern::get_transform_inverse() const {
        return transform_inverse;
    }

    // --- Factory Utilities ---
    Pattern solid_pattern(Color color){
        return {.type = PatternType::Solid, .a = color, .b = color};
    }

    Pattern stripe_pattern(Color a, Color b){
        return {.type = PatternType::Stripe, .a = a, .b = b};
    }

    Pattern gradient_pattern(Color a, Color b){
        return {.type = PatternType::Gradient, .a = a, .b = b};
    }

    Pattern ring_pattern(Color a, Color b){
        return {.type = PatternType::Ring, .a = a, .b = b};
    }

    Pattern checkers_pattern(Color a, Color b){
        return {.type = PatternType::Checkers, .a = a, .b = b};
    }

    Pattern test_pattern(){
        return {.type = PatternType::Test};
    }

    // --- Nested Factory Utilities ---
    Pattern stripe_pattern(Pattern a, Pattern b){
        return nested(PatternType::Stripe, std::move(a), std::move(b));
    }

    Pattern gradient_pattern(Pattern a, Pattern b){
        return nested(PatternType::Gradient, std::move(a), std::move(b));
    }

    Pattern ring_pattern(Pattern a, Pattern b){
        return nested(PatternType::Ring, std::move(a), std::move(b));
    }

    Pattern checkers_pattern(Pattern a, Pattern b){
        return nested(PatternType::Checkers, std::move(a), std::move(b));
    }

    Pattern perturbed_pattern(Pattern pattern, float perturbation){
        Pattern perturbed{.type = PatternType::Perturbed, .perturbation = perturbation};
        perturbed.children.push_back(std::move(pattern));
        return perturbed;
    }

    // --- Evaluation ---
    Color pattern_at(const Pattern& pattern, Point pattern_point){
        switch (pattern.type) {
            case PatternType::Solid:
                return pattern.a;

            case PatternType::Stripe:
                return slotColor(pattern, isEven(pattern_point.x) ? 0 : 1, pattern_point);

            case PatternType::Gradient: {
                const Color a = slotColor(pattern, 0, pattern_point);
                const Color b = slotColor(pattern, 1, pattern_point);
                const float fraction = pattern_point.x - std::floor(pattern_point.x);
                return a + (b - a) * fraction;
            }

            case PatternType::Ring: {
                const float radius = std::sqrt(pattern_point.x * pattern_point.x + pattern_point.z * pattern_point.z);
                return slotColor(pattern, isEven(radius) ? 0 : 1, pattern_point);
            }

            case PatternType::Checkers: {
                const float sum = std::floor(pattern_point.x) + std::floor(pattern_point.y) + std::floor(pattern_point.z);
                return slotColor(pattern, isEven(sum) ? 0 : 1, pattern_point);
            }

            case PatternType::Perturbed: {
                if (pattern.children.empty()) {
                    throw std::runtime_error("A perturbed pattern needs a pattern to perturb");
                }

                // Three decorrelated noise samples jitter x, y and z independently
                const float x = pattern_point.x;
                const float y = pattern_point.y;
                const float z = pattern_point.z;
                const Point jittered(
                    x + perlin_noise(x, y, z) * pattern.perturbation,
                    y + perlin_noise(x + NOISE_OFFSET_Y, y + NOISE_OFFSET_Y, z + NOISE_OFFSET_Y) * pattern.perturbation,
                    z + perlin_noise(x + NOISE_OFFSET_Z, y + NOISE_OFFSET_Z, z + NOISE_OFFSET_Z) * pattern.perturbation);
                return childAt(pattern.children.front(), jittered);
            }

            case PatternType::Test:
                return {pattern_point.x, pattern_point.y, pattern_point.z};
        }

        throw std::runtime_error("Unknown pattern type in pattern_at");
    }

    Color pattern_at_shape(const Pattern& pattern, const Matrix<4>& object_inverse, Point world_point){
        const Point object_point = object_inverse * world_point;
        const Point pattern_point = pattern.get_transform_inverse() * object_point;
        return pattern_at(pattern, pattern_point);
    }
}
