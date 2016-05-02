#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

Material::Material(MaterialId i) : id(i)
{
    diffuse_colour = glm::vec3(1, 1, 1);
    specular_colour = glm::vec3(1, 1, 1);
    shininess = 0;
}

MaterialId Material::getId() const
{
    return id;
}

glm::vec3 Material::getDiffuseColour() const
{
    return diffuse_colour;
}

void Material::setDiffuseColour(glm::vec3 c)
{
    diffuse_colour = c;
}

glm::vec3 Material::getSpecularColour() const
{
    return specular_colour;
}

void Material::setSpecularColour(glm::vec3 c)
{
    specular_colour = c;
}

float Material::getShininess() const
{
    return shininess;
}

void Material::setShininess(float s)
{
    shininess = s;
}

bool Material::isShiny() const
{
    return shininess > 0;
}
