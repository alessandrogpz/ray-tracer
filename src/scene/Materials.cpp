module rt.materials;

import rt.colors;
import rt.utils;

namespace rt {

    Material::Material() = default;

    Material::Material(Color _color, double _ambient, double _diffuse, double _specular, double _shininess)
        : color(_color), ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess) {}

    bool Material::operator==(const Material& other) const {
        return color == other.color &&
               equal(ambient, other.ambient) &&
               equal(diffuse, other.diffuse) &&
               equal(specular, other.specular) &&
               equal(shininess, other.shininess);
    }

} // namespace rt
