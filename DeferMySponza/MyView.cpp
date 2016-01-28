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
	{
		tsl::IndexedMesh mesh;
		tsl::CreateCone(1.f, 1.f, 12, &mesh);
		tsl::ConvertPolygonsToTriangles(&mesh);

		light_cone_mesh_.element_count = mesh.index_array.size();

		glGenBuffers(1, &light_cone_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_cone_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			mesh.vertex_array.size() * sizeof(glm::vec3),
			mesh.vertex_array.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &light_cone_mesh_.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cone_mesh_.element_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh.index_array.size() * sizeof(unsigned int),
			mesh.index_array.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &light_cone_mesh_.vao);
		glBindVertexArray(light_cone_mesh_.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cone_mesh_.element_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_cone_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	//create the shaders

	//gbuffer shaders
	std::vector<std::string>gvertex_attrib = { "vertex_position", "vertex_normal"};
	std::vector<std::string>gfragment_attrib = { "fragment_position", "fragment_normal", "fragment_diffuse", "fragment_specular","fragment_shininess" };
	gbuffer_prog = glCreateProgram();
	CreateShader(gbuffer_prog,gbuffer_vertex_shader, "gbuffer_vs.glsl", gvertex_attrib, GL_VERTEX_SHADER);
	CreateShader(gbuffer_prog, gbuffer_fragment_shader, "gbuffer_fs.glsl", std::vector<std::string>(), GL_FRAGMENT_SHADER, gfragment_attrib);
	glLinkProgram(gbuffer_prog);

	//ambient shaders
	std::vector<std::string>avertex_attrib = { "vertex_position", "vertex_normal" };
	std::vector<std::string>afragment_attrib = { "fragment_position", "fragment_normal" };
	ambient_prog = glCreateProgram();
	CreateShader(ambient_prog, gbuffer_vertex_shader, "ambient_vs.glsl", avertex_attrib, GL_VERTEX_SHADER);
	CreateShader(ambient_prog, gbuffer_fragment_shader, "ambient_fs.glsl", afragment_attrib, GL_FRAGMENT_SHADER);
	glLinkProgram(ambient_prog);
	//light shaders
	std::vector<std::string>lvertex_attrib = { "vertex_position", "vertex_normal" };
	std::vector<std::string>lfragment_attrib = { "fragment_position", "fragment_normal" };
	light_prog = glCreateProgram();
	CreateShader(light_prog, gbuffer_vertex_shader, "light_vs.glsl", lvertex_attrib, GL_VERTEX_SHADER);
	CreateShader(light_prog, gbuffer_fragment_shader, "light_fs.glsl", lfragment_attrib, GL_FRAGMENT_SHADER);
	glLinkProgram(light_prog);
	geometry_ = std::make_shared<SceneModel::GeometryBuilder>();
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
	
	Instance_BO_bufferIndex = glGetUniformBlockIndex(gbuffer_prog, "Instance");
	Instance_BO_bufferpointindex = 1;
	glGenBuffers(1, &Instance_BO);
	glBindBuffer(GL_UNIFORM_BUFFER, Instance_BO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Per_Instance)*maximum_instance, nullptr, GL_STREAM_DRAW);
	glUniformBlockBinding(gbuffer_prog, Instance_BO_bufferIndex, Instance_BO_bufferpointindex);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//UpdateLights(true);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totaloffset, nullptr, GL_STATIC_DRAW);
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
	GLuint position_location = glGetAttribLocation(gbuffer_prog, "vertex_position");
	GLuint normal_location = glGetAttribLocation(gbuffer_prog, "vertex_normal");

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

	glGenFramebuffers(1, &lbuffer_fbo);
	glGenFramebuffers(1, &gbuffer_fbo);
	glGenRenderbuffers(1, &lbuffer_colour_rbo);
	glGenTextures(1, &gbuffer_position_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glGenTextures(1, &gbuffer_normal_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glGenTextures(1, &gbuffer_diffuse_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glGenTextures(1, &gbuffer_specular_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glGenTextures(1, &gbuffer_shininess_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
}

void MyView::
windowViewDidReset(std::shared_ptr<tygra::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
	// build framebuffers build here to ensure resizing causes no issues
	GLenum framebuffer_status = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	// TODO: attach textures and buffers here

	glBindRenderbuffer(GL_RENDERBUFFER, lbuffer_colour_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, lbuffer_colour_rbo);

	framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, gbuffer_position_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, gbuffer_normal_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, gbuffer_specular_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex, 0);/**/

	GLenum Buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, Buffers);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);

	framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "framebuffer not complete");
	}
	

}

void MyView::
windowViewDidStop(std::shared_ptr<tygra::Window> window)
{
}

void MyView::
windowViewRender(std::shared_ptr<tygra::Window> window)
{
    assert(scene_ != nullptr);

    

	auto camera = scene_->getCamera();
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	auto aspect = Viewport[2] / (float)Viewport[3];
	auto view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getDirection(), scene_->getUpDirection());
	auto projection = glm::perspective(camera.getVerticalFieldOfViewInDegrees(), aspect, camera.getNearPlaneDistance(), camera.getFarPlaneDistance());

	//UpdateLights(false);
	// draw to GBuffer
	gbufferPass(view,projection);
	//ambient directional lights pass back buffer
	//ambientPass(view, projection);
	//light pass back buffer
	//LightPass(view, projection);
	//post

	//switch buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, lbuffer_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, Viewport[2], Viewport[3], 0, 0, Viewport[2], Viewport[3], GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
void MyView::gbufferPass(glm::mat4 view, glm::mat4 projection)
{
	// set framebuffer to gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	//clear depth stencil
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	//stencil write to 128
	glStencilMask(128);
	//set depth less than
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	//disable blend
	glDisable(GL_BLEND);
	//bind texture units to 0
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);*/
	//loop drawing
	
	glUseProgram(gbuffer_prog);
	SetUniforms(gbuffer_prog, view, projection);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, Instance_BO_bufferpointindex, Instance_BO);
	for (auto i : geometry_->getAllMeshes())
	{
		auto mesh = mesh_.find(i.getId())->second;
		auto instances = scene_->getInstancesByMeshId(i.getId());
		auto totaloffset = 0;
		for (auto j : instances)
		{
			Per_Instance* currentval = new Per_Instance(per_instance_.find(j)->second);
			glBufferSubData(GL_UNIFORM_BUFFER, totaloffset, sizeof(Per_Instance), &currentval);
			totaloffset += sizeof(Per_Instance);
		}
		
		glDrawElementsInstanced(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(mesh.element_offset), instances.size());
	}
}

void MyView::ambientPass(glm::mat4 view, glm::mat4 projection)
{
	//bind lbuffer as framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	//clear colour attachments to background
	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	//set stencil test to = 128 NO WRITE
	glStencilFunc(GL_EQUAL, 128, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// disable depth test
	glDisable(GL_DEPTH_TEST);
	// disable blend
	glDisable(GL_BLEND);
	//bind gbuffer textures
	glUseProgram(ambient_prog);
	GLint ambientlight = glGetUniformLocation(ambient_prog, "ambient_light");
	glUniform3fv(ambientlight,1,glm::value_ptr(scene_->getAmbientLightIntensity()));
	SetUniforms(ambient_prog, view, projection);

	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);

	glBindVertexArray(light_quad_mesh_.vao);
	// draw quad
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // draw quad

}
void MyView::LightPass(glm::mat4 view, glm::mat4 projection)
{
	//bind lbuffer as framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	// set stencil to equal 128 NO WRITE
	glStencilFunc(GL_EQUAL, 128, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//disable depth
	glDisable(GL_DEPTH_TEST);
	//enable blend to addative
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	//bind gbuffer textures to tex units
	glUseProgram(light_prog);
	GLint sampposloc = glGetUniformLocation(light_prog, "sampler_world_position");
	GLint sampnorloc = glGetUniformLocation(light_prog, "sampler_world_normal");
	GLint sampdifloc = glGetUniformLocation(light_prog, "sampler_world_diffuse");
	GLint sampspeloc = glGetUniformLocation(light_prog, "sampler_world_specular");
	GLint sampshiloc = glGetUniformLocation(light_prog, "sampler_world_shininess");
	glUniform1ui(sampposloc, 1);
	glUniform1ui(sampnorloc, 1);
	glUniform1ui(sampdifloc, 1);
	glUniform1ui(sampspeloc, 1);
	glUniform1ui(sampshiloc, 1);
	SetUniforms(gbuffer_prog, view, projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	//loop through light models
	// load all dir lights
	int offset = 0;
	for (auto i : scene_->getAllDirectionalLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Lights), &lights_[i.getId()]);
		offset += sizeof(Lights);
	}
	glBindVertexArray(light_quad_mesh_.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_quad_mesh_.element_vbo);
	glDrawElementsInstanced(GL_TRIANGLES, light_quad_mesh_.element_count, GL_UNSIGNED_INT, 0, scene_->getAllDirectionalLights().size());
	offset = 0;
	for (auto i : scene_->getAllPointLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER,offset,sizeof(Lights), &lights_[i.getId()]);
		offset += sizeof(Lights);
	}
	glBindVertexArray(light_sphere_mesh_.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_sphere_mesh_.element_vbo);
	glDrawElementsInstanced(GL_TRIANGLES,light_sphere_mesh_.element_count,GL_UNSIGNED_INT,0,scene_->getAllPointLights().size());
	offset = 0;
	for (auto i : scene_->getAllSpotLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Lights), &lights_[i.getId()]);
		offset += sizeof(Lights);
	}
	glBindVertexArray(light_cone_mesh_.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cone_mesh_.element_vbo);
	glDrawElementsInstanced(GL_TRIANGLES, light_cone_mesh_.element_count, GL_UNSIGNED_INT, 0, scene_->getAllSpotLights().size());
	
	
	//auto totaloffset = 0;
	//	glBindBuffer(GL_UNIFORM_BUFFER, Instance_BO);
	//	glBufferSubData(GL_UNIFORM_BUFFER, totaloffset, sizeof(Per_Instance), &per_instance_.find(j)->second);
	//	totaloffset += sizeof(Per_Instance);
	//glDrawElementsInstanced(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(element_offset), instances.size());

}
void MyView::SetUniforms(GLuint shader, glm::mat4 view_xform, glm::mat4 projection_xform)
{
	glUseProgram(shader);

	GLuint projection_xform_id = glGetUniformLocation(shader, "projection_xform");
	GLuint view_xform_id = glGetUniformLocation(shader, "view_xform");

	glUniformMatrix4fv(view_xform_id, 1, GL_FALSE, glm::value_ptr(view_xform));
	glUniformMatrix4fv(projection_xform_id, 1, GL_FALSE, glm::value_ptr(projection_xform));
}
void MyView::UpdateLights(bool firstrun)
{
	// store all light data
	for (auto i : scene_->getAllDirectionalLights())
	{
		Lights newlight;
		newlight.direction = i.getDirection();
		newlight.intensity = i.getIntensity();
		
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
	if (firstrun)
	{
		int maxinstance = 0;
		if (scene_->getAllDirectionalLights().size() > maxinstance)
			maxinstance = scene_->getAllDirectionalLights().size();
		if (scene_->getAllPointLights().size() > maxinstance)
			maxinstance = scene_->getAllPointLights().size();
		if (scene_->getAllSpotLights().size() > maxinstance)
			maxinstance = scene_->getAllSpotLights().size();

		glGenBuffers(1, &Light_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, Light_UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights)*maxinstance, nullptr, GL_STREAM_DRAW);
	}
}
bool MyView::CreateShader(GLuint shader_program,
	GLuint shader,
	std::string shader_name,
	std::vector<std::string> attriblocations,
	GLenum shadertype,
	std::vector<std::string> fragdatalocations)
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
	for (int i = 0; i < attriblocations.size(); i++)
		glBindFragDataLocation(shader_program, i, attriblocations[i].data());

	glDeleteShader(shader);
	return true;
}
