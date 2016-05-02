#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class SpotLight
{
public:
    SpotLight(LightId);

    LightId getId() const;

    bool isStatic() const;
    void setStatic(bool);

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 p);

    glm::vec3 getDirection() const;
    void setDirection(glm::vec3 d);

    float getConeAngleDegrees() const;
    void setConeAngleDegrees(float a);

    float getRange() const;
    void setRange(float r);

    glm::vec3 getIntensity() const;
    void setIntensity(glm::vec3 i);

    bool getCastShadow() const;
    void setCastShadow(bool);

private:
    glm::vec3 position{ 0, 0, 0 };
    float range{ 1 };
    glm::vec3 direction{ 0, 0, -1 };
    float cone_angle_degrees{ 45 };
    glm::vec3 intensity{ 1, 1, 0.8f };
    bool is_static{ false };
    bool cast_shadow{ true };
    LightId id;

};

} // end namespace SceneModel
