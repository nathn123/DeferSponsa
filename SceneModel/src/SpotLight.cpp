#include <SceneModel/SceneModel.hpp>

using namespace SceneModel;

SpotLight::SpotLight(LightId i) : id(i)
{
}

LightId SpotLight::getId() const
{
    return id;
}

bool SpotLight::isStatic() const
{
    return is_static;
}

void SpotLight::setStatic(bool b)
{
    is_static = b;
}

glm::vec3 SpotLight::getPosition() const
{
    return position;
}

void SpotLight::setPosition(glm::vec3 p)
{
    position = p;
}

glm::vec3 SpotLight::getDirection() const
{
    return direction;
}

void SpotLight::setDirection(glm::vec3 d)
{
    direction = d;
}

float SpotLight::getConeAngleDegrees() const
{
    return cone_angle_degrees;
}

void SpotLight::setConeAngleDegrees(float a)
{
    cone_angle_degrees = a;
}

float SpotLight::getRange() const
{
    return range;
}

void SpotLight::setRange(float r)
{
    range = r;
}

glm::vec3 SpotLight::getIntensity() const
{
    return intensity;
}

void SpotLight::setIntensity(glm::vec3 i)
{
    intensity = i;
}

bool SpotLight::getCastShadow() const
{
    return cast_shadow;
}

void SpotLight::setCastShadow(bool s)
{
    cast_shadow = s;
}
