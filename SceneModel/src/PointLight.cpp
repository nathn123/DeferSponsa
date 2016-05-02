#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

PointLight::PointLight(LightId i) : id(i)
{
}

LightId PointLight::getId() const
{
    return id;
}

bool PointLight::isStatic() const
{
    return is_static;
}

void PointLight::setStatic(bool b)
{
    is_static = b;
}

glm::vec3 PointLight::getPosition() const
{
    return position;
}

void PointLight::setPosition(glm::vec3 p)
{
    position = p;
}

float PointLight::getRange() const
{
    return range;
}

void PointLight::setRange(float r)
{
    range = r;
}

glm::vec3 PointLight::getIntensity() const
{
    return intensity;
}

void PointLight::setIntensity(glm::vec3 i)
{
    intensity = i;
}
