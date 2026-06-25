export module rt.materials;

import rt.colors;
import rt.utils;

export namespace rt
{
    struct Material
    {
        Color color{Color()};
        double ambient{0.1};
        double diffuse{0.9};
        double specular{0.9};
        double shininess{200.0};

        Material() = default;
        Material(Color _color, double _ambient, double _diffuse, double _specular, double _shininess)
            : color(_color), ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess) {}

        [[nodiscard]]
        bool operator==(const Material& other) const {
            return color == other.color &&
                   equal(ambient, other.ambient) &&
                   equal(diffuse, other.diffuse) &&
                   equal(specular, other.specular) &&
                   equal(shininess, other.shininess);
        }
    };
}