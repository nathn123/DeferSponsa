#pragma once

#include "SceneModel_fwd.hpp"
#include <glm/glm.hpp>

namespace SceneModel
{

class Instance
{
public:
    Instance(InstanceId i);

    InstanceId getId() const;

    bool isStatic() const;

    MeshId getMeshId() const;

    MaterialId getMaterialId() const;

    glm::mat4x3 getTransformationMatrix() const;

    void setStatic(bool b);
    void setMeshId(MeshId id);
    void setMaterialId(MaterialId id);
    void setTransformationMatrix(glm::mat4x3 m);

private:
    InstanceId id;
    MeshId mesh_id;
    MaterialId material_id;
    glm::mat4x3 xform;
    bool is_static;

};

} // end namespace SceneModel
