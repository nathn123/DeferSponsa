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
	struct Lights
	{
		glm::vec3 position;
		float range;
		glm::vec3 intensity;
		float coneangledegrees;
		glm::vec3 direction;
		int pad;
		bool shadows;

		Lights() :
			position(0),
			intensity(0),
			direction(0),
			range(0),
			coneangledegrees(0),
			shadows(false) {}
	};
	struct Per_Instance
	{
		glm::mat4 Model_xform;
		glm::vec3 specular;
		float shininess;
		glm::vec3 diffuse;

		Per_Instance() :
			Model_xform(0),
			specular(0),
			diffuse(0),
			shininess(0){}
	};
	void
		gbufferPass(glm::mat4 view, glm::mat4 projection);
	void
		ambientPass(glm::mat4 view, glm::mat4 projection);
	void
		LightPass(glm::mat4 view, glm::mat4 projection);
	void
		SetUniforms(GLuint shader, glm::mat4 View, glm::mat4 Projection);
	void
		UpdateLights(bool firstrun);

	bool
		CreateShader(GLuint shader_program, GLuint shader, std::string shader_name, std::vector<std::string> attriblocations, GLenum shadertype, std::vector<std::string> fragdatalocations = std::vector<std::string>());

	std::shared_ptr<const SceneModel::Context> scene_;
	std::shared_ptr<SceneModel::GeometryBuilder> geometry_;

	LightMesh light_quad_mesh_; 
	LightMesh light_sphere_mesh_;
	LightMesh light_cone_mesh_;
	GLuint gbuffer_prog;
	GLuint ambient_prog;
	GLuint light_prog;
	GLuint gbuffer_vertex_shader;
	GLuint gbuffer_fragment_shader;
	GLuint ambient_vertex_shader;
	GLuint ambient_frag_shader;
	GLuint light_vertex_shader;
	GLuint light_frag_shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint Light_UBO;
	GLuint Instance_BO;
	GLuint Instance_BO_bufferIndex;
	GLuint Instance_BO_bufferpointindex;
	GLuint lbuffer_fbo;
	GLuint lbuffer_colour_rbo;
	GLuint gbuffer_position_tex;
	GLuint gbuffer_normal_tex;
	GLuint gbuffer_diffuse_tex;
	GLuint gbuffer_specular_tex;
	GLuint gbuffer_shininess_tex;
	GLuint gbuffer_fbo;

	std::unordered_map<SceneModel::MeshId, Mesh> mesh_;
	std::unordered_map<SceneModel::InstanceId, Per_Instance> per_instance_;
	std::unordered_map<SceneModel::LightId, Lights> lights_;

};
