export module rt.materials;

import rt.colors;

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
    };
}