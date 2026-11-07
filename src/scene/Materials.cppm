export module rt.materials;

import std;
import rt.colors;
import rt.patterns;

export namespace rt {

    struct Material {
        Color color{Color()};
        float ambient{0.1f};
        float diffuse{0.9f};
        float specular{0.9f};
        float shininess{200.0f};
        float reflective{0.0f};             ///< 0 = matte, 1 = perfect mirror
        float transparency{0.0f};           ///< 0 = opaque, 1 = fully transparent
        float refractive_index{1.0f};       ///< 1 = vacuum, 1.5 = glass, 2.417 = diamond
        bool casts_shadow{true};            ///< false lets light pass, e.g. for a pond's water surface
        std::optional<Pattern> pattern{};

        Material();
        Material(Color _color, float _ambient, float _diffuse, float _specular, float _shininess);

        [[nodiscard]] bool operator==(const Material& other) const;
    };

} // namespace rt
