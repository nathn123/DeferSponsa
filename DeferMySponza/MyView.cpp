#include "MyView.hpp"
#include <SceneModel/SceneModel.hpp>
#include <tygra/FileHelper.hpp>
#include <tsl/primitives.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>

MyView::
MyView()
{
}

MyView::
~MyView() {
}

void MyView::
setScene(std::shared_ptr<const SceneModel::Context> scene)
{
    scene_ = scene;
}

void MyView::
windowViewWillStart(std::shared_ptr<tygra::Window> window)
{
    assert(scene_ != nullptr);
	{
		std::vector<glm::vec2> vertices(4);
		vertices[0] = glm::vec2(-1, -1);
		vertices[1] = glm::vec2(1, -1);
		vertices[2] = glm::vec2(1, 1);
		vertices[3] = glm::vec2(-1, 1);

		glGenBuffers(1, &light_quad_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_quad_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(glm::vec2),
			vertices.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &light_quad_mesh_.vao);
		glBindVertexArray(light_quad_mesh_.vao);
		glBindBuffer(GL_ARRAY_BUFFER, light_quad_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	{
		tsl::IndexedMesh mesh;
		tsl::CreateSphere(1.f, 12, &mesh);
		tsl::ConvertPolygonsToTriangles(&mesh);

		light_sphere_mesh_.element_count = mesh.index_array.size();

		glGenBuffers(1, &light_sphere_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_sphere_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			mesh.vertex_array.size() * sizeof(glm::vec3),
			mesh.vertex_array.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &light_sphere_mesh_.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_sphere_mesh_.element_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh.index_array.size() * sizeof(unsigned int),
			mesh.index_array.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &light_sphere_mesh_.vao);
		glBindVertexArray(light_sphere_mesh_.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_sphere_mesh_.element_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_sphere_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	//create the shaders

	//gbuffer shaders
	std::vector<std::string>vertex_attrib = { "vertex_position", "vertex_normal" };
	std::vector<std::string>fragment_attrib = { "fragment_position","fragment_normal"};
	gbuffer_prog = glCreateProgram();
	CreateShader(gbuffer_prog,gbuffer_vertex_shader, "gbuffer_vs.glsl", vertex_attrib, GL_VERTEX_SHADER);
	CreateShader(gbuffer_prog, gbuffer_fragment_shader, "gbuffer_frag.glsl", fragment_attrib, GL_FRAGMENT_SHADER);
	glLinkProgram(gbuffer_prog);


	unsigned int totaloffset = 0;
	unsigned int totalelementoffset = 0;
	unsigned int maximum_instance = 0;
	for (auto i : geometry_->getAllMeshes())
	{
		Mesh newmesh;
		// calculate offsets
		newmesh.position_offset = totaloffset;
		newmesh.normal_offset = newmesh.position_offset + i.getPositionArray().size() *sizeof(glm::vec3);
		totaloffset = newmesh.normal_offset + i.getNormalArray().size() *sizeof(glm::vec3);
		newmesh.element_offset = totalelementoffset;
		totalelementoffset += i.getElementArray().size()* sizeof(unsigned int);
		newmesh.element_count = i.getElementArray().size();
		//assign ID
		mesh_.insert(std::make_pair(i.getId(), newmesh));
	}
	for (auto i : scene_->getAllInstances())
	{
		Per_Instance newinst;
		auto mat = scene_->getMaterialById(i.getMaterialId());
		newinst.Model_xform = glm::mat4(i.getTransformationMatrix());
		newinst.diffuse = mat.getDiffuseColour();
		newinst.shininess = mat.getShininess();
		newinst.specular = mat.getSpecularColour();
		per_instance_.insert(std::make_pair(i.getId(), newinst));

		if (scene_->getInstancesByMeshId(i.getMeshId()).size()>maximum_instance)
			maximum_instance = scene_->getInstancesByMeshId(i.getMeshId()).size();
	}

	// create empty buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totaloffset, nullptr, GL_STATIC_DRAW);
	glGenBuffers(1, &Instance_BO);
	glBindBuffer(GL_ARRAY_BUFFER, Instance_BO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Per_Instance)*maximum_instance, nullptr, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalelementoffset, nullptr, GL_STATIC_DRAW);
	for (auto i : mesh_)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		auto mesh = geometry_->getMeshById(i.first);
		//buffer the data
		// position data
		glBufferSubData(GL_ARRAY_BUFFER, i.second.position_offset,
			mesh.getPositionArray().size() * sizeof(glm::vec3),
			mesh.getPositionArray().data());
		//normal data
		glBufferSubData(GL_ARRAY_BUFFER, i.second.normal_offset,
			mesh.getNormalArray().size() * sizeof(glm::vec3),
			mesh.getNormalArray().data());
		//element data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, i.second.element_offset,
			mesh.getElementArray().size() * sizeof(unsigned int),
			mesh.getElementArray().data());
	}
	//generate VAO
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//set up attribute locations
	GLuint position_location = glGetAttribLocation(gbuffer_prog, "Position");
	GLuint normal_location = glGetAttribLocation(gbuffer_prog, "Normal");
	GLuint model_xform_location = glGetAttribLocation(gbuffer_prog, "Model_xform");

	/*GLuint model_xform_location_li = glGetAttribLocation(light_prog, "Model_xform");
	GLuint diffuse_location = glGetAttribLocation(light_prog, "Diffuse");
	GLuint specular_location = glGetAttribLocation(light_prog, "Specular");
	GLuint shininess_location = glGetAttribLocation(light_prog, "Shininess");
	GLuint shiny_location = glGetAttribLocation(light_prog, "Is_Shiny");*/

	//Enable attrib array
	glEnableVertexAttribArray(position_location);
	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glVertexAttribDivisor(position_location, 1); // this makes it instanced
	glEnableVertexAttribArray(normal_location);
	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(mesh_.begin()->second.normal_offset));
	glVertexAttribDivisor(normal_location, 1);
	glEnableVertexAttribArray(model_xform_location);
	glVertexAttribPointer(model_xform_location, 16, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), 0);
	glVertexAttribDivisor(model_xform_location, 1);

	//glEnableVertexAttribArray(model_xform_location_li);
	//glVertexAttribPointer(model_xform_location_li, 16, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), 0);
	//glVertexAttribDivisor(model_xform_location_li, 1);
	//glEnableVertexAttribArray(diffuse_location);
	//glVertexAttribPointer(diffuse_location, 3, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(float)));
	//glVertexAttribDivisor(diffuse_location, 1);
	//glEnableVertexAttribArray(specular_location);
	//glVertexAttribPointer(specular_location, 3, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::mat4)));
	//glVertexAttribDivisor(specular_location, 1);
	//glEnableVertexAttribArray(shininess_location);
	//glVertexAttribPointer(shininess_location, 1, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::mat4) + sizeof(glm::vec3)));
	//glVertexAttribDivisor(shininess_location, 1);

}

void MyView::
windowViewDidReset(std::shared_ptr<tygra::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
}

void MyView::
windowViewDidStop(std::shared_ptr<tygra::Window> window)
{
}

void MyView::
windowViewRender(std::shared_ptr<tygra::Window> window)
{
    assert(scene_ != nullptr);

    glClearColor(0.f, 0.f, 0.25f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

	auto camera = scene_->getCamera();
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	auto aspect = Viewport[2] / (float)Viewport[3];
	auto view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getDirection(), scene_->getUpDirection());
	auto projection = glm::perspective(camera.getVerticalFieldOfViewInDegrees(), aspect, camera.getNearPlaneDistance(), camera.getFarPlaneDistance());

	// set framebuffer to gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo_);
	// draw to GBuffer
	
	SetUniforms(gbuffer_prog, view, projection);
	glBindVertexArray(VAO);
	gbufferPass();
	//ambient / directional lights pass back buffer

	//light pass back buffer
	UpdateLights();

	//post

	//switch buffer
}
void MyView::gbufferPass()
{
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glUseProgram(gbuffer_prog);
	for (auto i : geometry_->getAllMeshes())
	{
		auto mesh = mesh_.find(i.getId())->second;
		auto instances = scene_->getInstancesByMeshId(i.getId());
		auto totaloffset = 0;
		for (auto j : instances)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, Instance_BO);
			glBufferSubData(GL_UNIFORM_BUFFER, totaloffset, sizeof(Per_Instance), &per_instance_.find(j)->second);
			totaloffset += sizeof(Per_Instance);
		}
		glDrawElementsInstanced(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(mesh.element_offset), instances.size());
	}
}

void MyView::ambientPass(unsigned int element_count, unsigned int element_offset, std::vector<SceneModel::InstanceId> instances)
{
	glUseProgram(ambient_prog);
	GLint sampposloc = glGetUniformLocation(ambient_prog, "sampler_world_position");
	GLint sampnorloc = glGetUniformLocation(ambient_prog, "sampler_world_normal");
	glUniform1ui(sampposloc, 1);
	glUniform1ui(sampnorloc, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex_);


	// load all dir lights
	scene_->getAllDirectionalLights().begin()->;
	GLint lightdirloc = glGetUniformLocation(ambient_prog, "light_direction");
	glUniform3fv(lightdirloc, 1, glm::value_ptr(global_light_direction));
	

	glBindVertexArray(light_quad_mesh_.vao);
	//stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // draw quad

}
void MyView::LightPass(Lights light, unsigned int element_count, unsigned int element_offset, std::vector<SceneModel::InstanceId> instances)
{
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_EQUAL);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindBuffer(GL_UNIFORM_BUFFER, Light_UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &light, GL_STREAM_DRAW);

	auto totaloffset = 0;
	for (auto j : instances)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Instance_BO);
		glBufferSubData(GL_UNIFORM_BUFFER, totaloffset, sizeof(Per_Instance), &per_instance_.find(j)->second);
		totaloffset += sizeof(Per_Instance);
	}
	glDrawElementsInstanced(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(element_offset), instances.size());

}
void MyView::SetUniforms(GLuint shader, glm::mat4 view_xform, glm::mat4 projection_xform)
{
	glUseProgram(shader);

	GLuint projection_xform_id = glGetUniformLocation(shader, "projection_xform");
	GLuint view_xform_id = glGetUniformLocation(shader, "view_xform");

	glUniformMatrix4fv(view_xform_id, 1, GL_FALSE, glm::value_ptr(view_xform));
	glUniformMatrix4fv(projection_xform_id, 1, GL_FALSE, glm::value_ptr(projection_xform));
}
void MyView::UpdateLights()
{
	// store all light data
	for (auto i : scene_->getAllDirectionalLights())
	{
		Lights newlight;
		newlight.direction = i.getDirection();
		newlight.intensity = i.getIntensity();
		newlight.is_direction = true;
		lights_.insert(std::make_pair(i.getId(), newlight));
	}
	for (auto i : scene_->getAllPointLights())
	{
		Lights newlight;
		newlight.position = i.getPosition();
		newlight.intensity = i.getIntensity();
		newlight.range = i.getRange();
		lights_.insert(std::make_pair(i.getId(), newlight));
	}
	for (auto i : scene_->getAllSpotLights())
	{
		Lights newlight;
		newlight.position = i.getPosition();
		newlight.intensity = i.getIntensity();
		newlight.range = i.getRange();
		newlight.shadows = i.getCastShadow();
		newlight.coneangledegrees = i.getConeAngleDegrees();
		newlight.direction = i.getDirection();
		lights_.insert(std::make_pair(i.getId(), newlight));
	}
}
bool MyView::CreateShader(GLuint shader_program,
	GLuint shader,
	std::string shader_name,
	std::vector<std::string> attriblocations,
	GLenum shadertype)
{
	GLint compile_status = 0;

	shader = glCreateShader(shadertype); // create shader
	std::string vertex_shader_string = tygra::stringFromFile(shader_name); // load shader file
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(shader, 1, (const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetShaderInfoLog(shader, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}

	// attach shaders to the program
	glAttachShader(shader_program, shader);
	for (int i = 0; i < attriblocations.size(); i++)
		glBindAttribLocation(shader_program, i, attriblocations[i].data());

	glDeleteShader(shader);
	return true;
}
