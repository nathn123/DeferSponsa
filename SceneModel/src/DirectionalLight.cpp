#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

DirectionalLight::DirectionalLight(LightId i) : id(i)
{
}

LightId DirectionalLight::getId() const
{
    return id;
}

bool DirectionalLight::isStatic() const
{
    return is_static;
}

void DirectionalLight::setStatic(bool b)
{
    is_static = b;
}

glm::vec3 DirectionalLight::getDirection() const
{
    return direction;
}

void DirectionalLight::setDirection(glm::vec3 d)
{
    direction = d;
}

glm::vec3 DirectionalLight::getIntensity() const
{
    return intensity;
}

void DirectionalLight::setIntensity(glm::vec3 i)
{
    intensity = i;
}
