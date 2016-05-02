#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class DirectionalLight
{
public:
    DirectionalLight(LightId);

    LightId getId() const;

    bool isStatic() const;
    void setStatic(bool);

    glm::vec3 getDirection() const;
    void setDirection(glm::vec3 d);

    glm::vec3 getIntensity() const;
    void setIntensity(glm::vec3 i);

private:
    glm::vec3 direction{ 0, 0, -1 };
    bool is_static{ true };
    glm::vec3 intensity{ 0.1f, 0.1f, 0.1f };
    LightId id;

};

} // end namespace SceneModel
