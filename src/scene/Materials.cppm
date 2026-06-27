export module rt.materials;

import rt.colors;

export namespace rt {

    struct Material {
        Color color{Color()};
        float ambient{0.1f};
        float diffuse{0.9f};
        float specular{0.9f};
        float shininess{200.0f};

        Material();
        Material(Color _color, float _ambient, float _diffuse, float _specular, float _shininess);

        [[nodiscard]] bool operator==(const Material& other) const;
    };

} // namespace rt
