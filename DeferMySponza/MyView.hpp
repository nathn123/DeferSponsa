#pragma once

#include <SceneModel/SceneModel_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <memory>

class MyView : public tygra::WindowViewDelegate
{
public:

    MyView();

    ~MyView();

    void
    setScene(std::shared_ptr<const SceneModel::Context> scene);

private:

    void
    windowViewWillStart(std::shared_ptr<tygra::Window> window) override;

    void
    windowViewDidReset(std::shared_ptr<tygra::Window> window,
                       int width,
                       int height) override;

    void
    windowViewDidStop(std::shared_ptr<tygra::Window> window) override;

    void
    windowViewRender(std::shared_ptr<tygra::Window> window) override;


	struct Mesh
	{
		unsigned int position_offset;
		unsigned int normal_offset;
		unsigned int element_offset;
		unsigned int element_count;

		Mesh() :
			position_offset(0),
			normal_offset(0),
			element_offset(0),
			element_count(0){}
	};
	struct LightMesh
	{
		GLuint vertex_vbo;
		GLuint element_vbo;
		GLuint vao;
		int element_count;

		LightMesh() : vertex_vbo(0),
			element_vbo(0),
			vao(0),
			element_count(0) {}
	};
	//OldLight JiC
	//struct Lights
	//{
	//	glm::vec3 position;
	//	float range;
	//	glm::vec3 intensity;
	//	float coneangledegrees;
	//	glm::vec3 direction;
	//	int shadows;

	//	Lights() :
	//		position(0),
	//		intensity(0),
	//		direction(0),
	//		range(0),
	//		coneangledegrees(0),
	//		shadows(-1) {}
	//};
	struct Lights
	{
		glm::mat4 Modelxform;
		glm::vec3 position;
		float range;
		glm::vec3 intensity;
		float coneangledegrees;
		glm::vec3 direction;
		int shadows;

		Lights() :
			Modelxform(0),
			position(0),
			range(0),
			intensity(0),
			coneangledegrees(0),
			direction(0),
			shadows(-1) {}
	};
	struct Per_Instance
	{
		glm::mat4 Model_xform;
		glm::vec3 specular;
		float shininess;
		glm::vec3 diffuse;
		float padding;

		Per_Instance() :
			Model_xform(0),
			specular(0),
			diffuse(0),
			shininess(0),
			padding(0){}
	};
	void
		gbufferPass(glm::mat4 view, glm::mat4 projection);
	void
		ambientPass(glm::mat4 view, glm::mat4 projection);
	void
		LightPass(glm::mat4 view, glm::mat4 projection);
	void
		AAPass();
	void
		AAEdgePass();
	void
		AABlendPass();
	void
		AANeighbourPass();
	void
		ShadowPass();
	void
		SetUniforms(GLuint shader, glm::mat4 View, glm::mat4 Projection);
	void
		UpdateLights();

	bool
		CreateShader(GLuint& shader_program, std::string shader_name, std::vector<std::string> attriblocations, std::vector<std::string> fragdatalocations);

	std::shared_ptr<const SceneModel::Context> scene_;
	std::shared_ptr<SceneModel::GeometryBuilder> geometry_;

	LightMesh light_quad_mesh_; 
	LightMesh light_sphere_mesh_;
	LightMesh light_cone_mesh_;
	GLuint gbuffer_prog;
	GLuint ambient_prog;
	GLuint light_prog;
	GLuint Dir_prog;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint Light_BO;
	GLuint IBO;
	GLuint lbuffer_fbo;
	GLuint lbuffer_colour_rbo;
	GLuint gbuffer_position_tex;
	GLuint gbuffer_normal_tex;
	GLuint gbuffer_diffuse_tex;
	GLuint gbuffer_specular_tex;
	GLuint gbuffer_shininess_tex;
	GLuint gbuffer_depth_tex;
	GLuint gbuffer_depth_stencil_RB;
	GLuint gbuffer_fbo;

	//SMAA vars
	GLuint SMAA_prog;
	GLuint SMAA_InputTex;
	GLuint SMAA_Blendprog;
	GLuint SMAA_Edgeprog;
	GLuint SMAA_edge_fbo;
	GLuint SMAA_blend_fbo;
	GLuint SMAA_Neighbourprog;
	GLuint SMAA_Fbuffer;
	GLuint SMAA_edgeTex;
	GLuint SMAA_blendTex;
	GLuint SMAA_areaTex;
	GLuint SMAA_seachTex;

	std::unordered_map<SceneModel::MeshId, Mesh> mesh_;
	std::unordered_map<SceneModel::InstanceId, Per_Instance> per_instance_;
	std::unordered_map<SceneModel::LightId, Lights> lights_;

};
