#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class Material
{
public:
    Material(MaterialId i);

    MaterialId getId() const;

    bool isStatic() const { return true; }

    glm::vec3 getDiffuseColour() const;
    void setDiffuseColour(glm::vec3 c);

    glm::vec3 getSpecularColour() const;
    void setSpecularColour(glm::vec3 c);

    float getShininess() const;
    void setShininess(float s);

    bool isShiny() const;


private:
    MaterialId id;
    glm::vec3 diffuse_colour;
    glm::vec3 specular_colour;
    float shininess;

};

} // end namespace SceneModel
