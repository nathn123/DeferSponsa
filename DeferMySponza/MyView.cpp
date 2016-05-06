#include "MyView.hpp"
#include <SceneModel/SceneModel.hpp>
#include <tygra/FileHelper.hpp>
#include <tsl/primitives.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>
#include "Textures\AreaTex.h"
#include "Textures\SearchTex.h"
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
	std::vector<std::string>gvertex_attrib = { "vertex_position", "vertex_normal", "diffuse", "specular", "shininess", "model_xform" };
	std::vector<std::string>gfragment_attrib = { "fragment_position", "fragment_normal", "fragment_diffuse", "fragment_specular","fragment_shininess" };
	CreateShader(gbuffer_prog, "gbuffer", gvertex_attrib, gfragment_attrib);

	//ambient shaders
	std::vector<std::string>avertex_attrib = { "vertex_position"};
	std::vector<std::string>afragment_attrib = { "finalcolour"};
	CreateShader(ambient_prog,  "ambient", avertex_attrib, afragment_attrib);
	//dir shaders
	std::vector<std::string>dvertex_attrib = { "vertex_position" };
	std::vector<std::string>dfragment_attrib = { "fragment_colour" };
	CreateShader(Dir_prog, "dir", dvertex_attrib, dfragment_attrib);
	//light shaders
	std::vector<std::string>lvertex_attrib = { "vertex_position"};
	std::vector<std::string>lfragment_attrib = {/* "fragment_colour"*/};
	CreateShader(light_prog, "light", lvertex_attrib, lfragment_attrib);
	//shadow shaders
	std::vector<std::string>svertex_attrib = { "vertex_position" };
	std::vector<std::string>sfragment_attrib = {"fragment_colour" };
	CreateShader(Shadow_prog, "shadow", svertex_attrib, sfragment_attrib);
	std::vector<std::string>sPrevertex_attrib = { "vertex_position" };
	std::vector<std::string>sPrefragment_attrib = { "fragment_colour" };
	CreateShader(Shadow_Pre_Prog, "shadow_Pre", sPrevertex_attrib, sPrefragment_attrib);


	CreateShaderAA(SMAA_Blendprog, "SMAA_Blend");

	CreateShaderAA(SMAA_Edgeprog, "SMAA_Edge");

	CreateShaderAA(SMAA_Neighbourprog, "SMAA_Neighbour");

	CreateShaderAA(SMAA_Resolveprog, "SMAA_Resolve");



	geometry_ = std::make_shared<SceneModel::GeometryBuilder>();
	unsigned int totaloffset = 0;
	unsigned int totalelementoffset = 0;
	unsigned int maximum_instance = 0;
	std::vector<glm::vec3> Pos, Norm, VBODATA;
	std::vector<unsigned int> EBODATA;
	for (auto i : geometry_->getAllMeshes())
	{
		Mesh newmesh;
		// calculate offsets
		newmesh.position_offset = totaloffset;
		totaloffset += i.getPositionArray().size();

		newmesh.element_offset = totalelementoffset;
		totalelementoffset += i.getElementArray().size()* sizeof(unsigned int);
		newmesh.element_count = i.getElementArray().size();
		newmesh.id = i.getId();
		//assign ID
		mesh_.push_back( newmesh);
		Pos.insert(Pos.end(), i.getPositionArray().begin(), i.getPositionArray().end());
		Norm.insert(Norm.end(), i.getNormalArray().begin(), i.getNormalArray().end());
		//for some reson insert doesnt want to work
		//EBODATA.insert(EBODATA.end(), i.getElementArray().begin(), i.getElementArray().end());
		for each (auto ele in i.getElementArray())
			EBODATA.push_back(ele);
	}
	for (int i = 0; i < mesh_.size(); i++)
	{
		//if (i.isStatic())
		//	continue;
		std::vector<Per_Instance> MeshInstances;
		for (auto j : scene_->getInstancesByMeshId(mesh_[i].id))
		{
			auto p = scene_->getInstanceById(j);
			Per_Instance newinst = Per_Instance();
			
			auto mat = scene_->getMaterialById(p.getMaterialId());
			newinst.Model_xform = glm::mat4(p.getTransformationMatrix());
			newinst.diffuse = mat.getDiffuseColour();
			newinst.shininess = mat.getShininess();
			newinst.specular = mat.getSpecularColour();
			MeshInstances.push_back(newinst);
		}
		InstanceData.push_back(MeshInstances);
	}

	VBODATA.insert(VBODATA.end(),Pos.begin(),Pos.end());
	VBODATA.insert(VBODATA.end(), Norm.begin(), Norm.end());

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VBODATA.size() * sizeof(glm::vec3), VBODATA.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Per_Instance)* maximum_instance, nullptr, GL_STREAM_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBODATA.size() * sizeof(unsigned int), EBODATA.data(), GL_STATIC_DRAW);
	//generate VAO
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//set up attribute locations
	
	//Enable attrib array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), TGL_BUFFER_OFFSET(Pos.size()*sizeof(glm::vec3)));


	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::vec4)*4 + sizeof(glm::vec3) + sizeof(float)));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::vec4) * 4));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::vec4) * 4 + sizeof(glm::vec3)));
	glVertexAttribDivisor(4, 1);

	for (int i = 0; i < 4; ++i)
	{
		glEnableVertexAttribArray(5 + i);
		glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Per_Instance), TGL_BUFFER_OFFSET(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(5 + i, 1);
	}

	// create Light array buffer
	int maxinstance = 0;
	if (scene_->getAllDirectionalLights().size() > maxinstance)
		maxinstance = scene_->getAllDirectionalLights().size();
	if (scene_->getAllPointLights().size() > maxinstance)
		maxinstance = scene_->getAllPointLights().size();
	if (scene_->getAllSpotLights().size() > maxinstance)
		maxinstance = scene_->getAllSpotLights().size();
	glGenBuffers(1, &Light_BO);
	glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights)*maxinstance, nullptr, GL_STREAM_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, glGetUniformBlockIndex(light_prog, "Lights"), Light_BO);


	glGenFramebuffers(1, &lbuffer_fbo);
	glGenFramebuffers(1, &gbuffer_fbo);
	glGenTextures(1, &lbuffer_colour_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, lbuffer_colour_tex);
	glGenRenderbuffers(1, &lbuffer_depth_stencil_RB);
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
	glGenTextures(1, &gbuffer_depth_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_depth_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glGenRenderbuffers(1, &gbuffer_depth_stencil_RB);

	//SMAA Textures
	
	glGenTextures(1, &SMAA_InputTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_InputTex);
	glGenTextures(1, &SMAA_edgeTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_edgeTex);
	glGenTextures(1, &SMAA_blendTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_blendTex);
	glGenTextures(1, &SMAA_OutputTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_OutputTex);
	glGenTextures(1, &SMAA_DepthTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_DepthTex);
	glGenTextures(1, &SMAA_PrevTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_PrevTex);


	//SMAA LOADING PRECOMPILED TEXTURES
	glGenTextures(1, &SMAA_areaTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_areaTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, AREATEX_WIDTH, AREATEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, areaTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenTextures(1, &SMAA_seachTex);
	glBindTexture(GL_TEXTURE_2D, SMAA_seachTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, searchTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//SMAA FRAMEBUFFERS
	glGenFramebuffers(1, &SMAA_edge_fbo);
	glGenFramebuffers(1, &SMAA_blend_fbo);
	glGenFramebuffers(1, &SMAA_fbo);

	//Shadow Framebuffer
	glGenFramebuffers(1, &Shadow_fbo);
	glGenTextures(1, &Shadow_depth_tex);

	// Gen Queries
	glGenQueries(1, &Gbuffer_Query);
	FrameCount = 0;
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

	glBindTexture(GL_TEXTURE_RECTANGLE, lbuffer_colour_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE,0, GL_RGB32F, width, height,0,GL_RGB,GL_FLOAT,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, lbuffer_colour_tex, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, lbuffer_depth_stencil_RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lbuffer_depth_stencil_RB);
	GLenum Buffer[] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, Buffer);
	framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, gbuffer_position_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, gbuffer_normal_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, gbuffer_specular_tex, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex, 0);/**/
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, gbuffer_depth_stencil_RB);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gbuffer_depth_stencil_RB);
	

	GLenum Buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, Buffers);

	framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "framebuffer not complete");
	}

	//SMAA Render Targets
	GLenum SMAABuffers[] = { GL_COLOR_ATTACHMENT0 };
	
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_edge_fbo);
	glBindTexture(GL_TEXTURE_2D, SMAA_edgeTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SMAA_edgeTex, 0);
	glDrawBuffers(1, SMAABuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_blend_fbo);
	glBindTexture(GL_TEXTURE_2D, SMAA_blendTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SMAA_blendTex, 0);
	glDrawBuffers(1, SMAABuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_fbo);
	glBindTexture(GL_TEXTURE_2D, SMAA_OutputTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SMAA_OutputTex, 0);
	glDrawBuffers(1, SMAABuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glBindTexture(GL_TEXTURE_2D, SMAA_PrevTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// doesnt need to be attached
	glBindTexture(GL_TEXTURE_2D, SMAA_InputTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, SMAA_DepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	


	framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, Shadow_fbo);

	glBindTexture(GL_TEXTURE_RECTANGLE, Shadow_depth_tex);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, Shadow_depth_tex, 0);
	glDrawBuffers(0, nullptr);

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
	if (FrameCount == 2)
		FrameCount = 0;


	auto camera = scene_->getCamera();
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	auto aspect = Viewport[2] / (float)Viewport[3];
	auto view = glm::lookAt(camera.getPosition(), camera.getDirection(), scene_->getUpDirection());
	auto projection = glm::perspective(camera.getVerticalFieldOfViewInDegrees(), aspect, camera.getNearPlaneDistance(), camera.getFarPlaneDistance());

	UpdateLights();
	UpdateMeshs();
	// draw to GBuffer
	gbufferPass(view,projection);
	//blit depth and stencil
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lbuffer_fbo);
	glBlitFramebuffer(0, 0, Viewport[2], Viewport[3], 0, 0, Viewport[2], Viewport[3], GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, Viewport[2], Viewport[3], 0, 0, Viewport[2], Viewport[3], GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	ambientPass(view, projection);
	LightPass(view, projection);
	//post
	ShadowPass();

	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	glBindTexture(GL_TEXTURE_2D, SMAA_InputTex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Viewport[2], Viewport[3]); // copy final scene image to smaa input texture
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	glBindTexture(GL_TEXTURE_2D, SMAA_DepthTex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Viewport[2], Viewport[3]); // copy final scene depth to smaa input texture
	AAPass();

	//switch buffer

	


	glBindFramebuffer(GL_READ_FRAMEBUFFER, SMAA_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, Viewport[2], Viewport[3], 0, 0, Viewport[2], Viewport[3], GL_COLOR_BUFFER_BIT, GL_LINEAR);
	FrameCount++;
	FirstPass = true;
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, lbuffer_fbo);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, Viewport[2], Viewport[3], 0, 0, Viewport[2], Viewport[3], GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
void MyView::gbufferPass(glm::mat4 view, glm::mat4 projection)
{
	// set framebuffer to gbuffer


	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	//clear depth stencil
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glClearStencil(128);
	glClear(GL_STENCIL_BUFFER_BIT);
	//stencil write to 128
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glStencilMask(~0);
	//set depth less than

	//disable blend
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//loop drawing

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);


	glUseProgram(gbuffer_prog);
	SetUniforms(gbuffer_prog, view, projection);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	for (int i = 0; i < mesh_.size();i++)
	{
		glBufferData(GL_ARRAY_BUFFER, InstanceData[i].size()*sizeof(Per_Instance), InstanceData[i].data(), GL_STREAM_DRAW);
		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, mesh_[i].element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(mesh_[i].element_offset), InstanceData[i].size(), mesh_[i].position_offset);
	}

	glDisable(GL_CULL_FACE);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MyView::ambientPass(glm::mat4 view, glm::mat4 projection)
{
	//bind lbuffer as framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	//clear colour attachments to background
	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);
	// disable depth test
	glDisable(GL_DEPTH_TEST);
	// disable blend
	glDisable(GL_BLEND);
	//bind gbuffer textures
	glUseProgram(ambient_prog);
	GLint ambientlight = glGetUniformLocation(ambient_prog, "ambient_light");
	glUniform3fv(ambientlight,1,glm::value_ptr(scene_->getAmbientLightIntensity()));
	SetUniforms(ambient_prog, view, projection);
	// draw quad
	glBindVertexArray(light_quad_mesh_.vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	glUseProgram(0);
	//for (auto i : scene_->getAllDirectionalLights())
	//{
	//		glm::mat4 Matrix = glm::mat4(1);
	//		Lights newlight;
	//		newlight.Modelxform = Matrix;
	//		newlight.intensity = i.getIntensity();
	//		newlight.direction = i.getDirection();
	//	glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
	//	glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &newlight, GL_STREAM_DRAW);
	//	glBindVertexArray(light_quad_mesh_.vao);
	//	glDrawElements(GL_TRIANGLES, light_quad_mesh_.element_count, GL_UNSIGNED_INT, 0);
	//}
}
void MyView::LightPass(glm::mat4 view, glm::mat4 projection)
{
	GLint sampposloc, sampnorloc, sampdifloc,sampspeloc, sampshiloc;
	//bind lbuffer as framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);

	// pre write to stencil
	glEnable(GL_STENCIL_TEST);
	glStencilMask(127);
	glStencilFunc(GL_NOTEQUAL, 128, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(light_prog);
	sampposloc = glGetUniformLocation(light_prog, "sampler_world_position");
	sampnorloc = glGetUniformLocation(light_prog, "sampler_world_normal");
	sampdifloc = glGetUniformLocation(light_prog, "sampler_world_diffuse");
	sampspeloc = glGetUniformLocation(light_prog, "sampler_world_specular");
	sampshiloc = glGetUniformLocation(light_prog, "sampler_world_shininess");
	SetUniforms(light_prog, view, projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glUniform1i(sampposloc, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glUniform1i(sampnorloc, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glUniform1i(sampdifloc, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glUniform1i(sampspeloc, 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glUniform1i(sampshiloc, 4);

	for (auto i : scene_->getAllPointLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
		auto test = lights_[i.getId()];
		glm::mat4 Matrix = glm::mat4(1);
		Matrix = glm::translate(Matrix, i.getPosition());
		Matrix = glm::scale(Matrix, glm::vec3(i.getRange()));
		Lights newlight;
		newlight.Modelxform = Matrix;
		newlight.position = i.getPosition();
		newlight.intensity = i.getIntensity();
		newlight.range = i.getRange();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &newlight, GL_STREAM_DRAW);
		glBindVertexArray(light_sphere_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_sphere_mesh_.element_count, GL_UNSIGNED_INT, 0);
	}


	for (auto i : scene_->getAllSpotLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &lights_[i.getId()], GL_STREAM_DRAW);
		glBindVertexArray(light_cone_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_cone_mesh_.element_count, GL_UNSIGNED_INT, 0);
	}
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//disable depth
	glDisable(GL_DEPTH_TEST);
	//enable blend to addative
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//bind gbuffer textures to tex units
	glUseProgram(Dir_prog);
	 sampposloc = glGetUniformLocation(Dir_prog, "sampler_world_position");
	 sampnorloc = glGetUniformLocation(Dir_prog, "sampler_world_normal");
	 sampdifloc = glGetUniformLocation(Dir_prog, "sampler_world_diffuse");
	 sampspeloc = glGetUniformLocation(Dir_prog, "sampler_world_specular");
	 sampshiloc = glGetUniformLocation(Dir_prog, "sampler_world_shininess");

	SetUniforms(Dir_prog, view, projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glUniform1i(sampposloc, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glUniform1i(sampnorloc, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glUniform1i(sampdifloc, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glUniform1i(sampspeloc, 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glUniform1i(sampshiloc, 4);

	for (auto i : scene_->getAllDirectionalLights())
	{
		glm::mat4 Matrix = glm::mat4(1);
		glUniformMatrix4fv(glGetUniformLocation(Dir_prog, "Modelxform"), 1, false, glm::value_ptr(Matrix));
		glUniform3fv(glGetUniformLocation(Dir_prog, "Intensity"), 1, glm::value_ptr(i.getIntensity()));
		glUniform3fv(glGetUniformLocation(Dir_prog, "Direction"), 1, glm::value_ptr(i.getDirection()));
		glBindVertexArray(light_quad_mesh_.vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	//cull front faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GREATER);
	glStencilMask(127);
	glStencilFunc(GL_EQUAL, 1, 127);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);



	glUseProgram(light_prog);
	sampposloc = glGetUniformLocation(light_prog, "sampler_world_position");
	sampnorloc = glGetUniformLocation(light_prog, "sampler_world_normal");
	sampdifloc = glGetUniformLocation(light_prog, "sampler_world_diffuse");
	sampspeloc = glGetUniformLocation(light_prog, "sampler_world_specular");
	sampshiloc = glGetUniformLocation(light_prog, "sampler_world_shininess");
	SetUniforms(light_prog, view, projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_tex);
	glUniform1i(sampposloc, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_tex);
	glUniform1i(sampnorloc, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_diffuse_tex);
	glUniform1i(sampdifloc, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_specular_tex);
	glUniform1i(sampspeloc, 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_shininess_tex);
	glUniform1i(sampshiloc, 4);

	for (auto i : scene_->getAllPointLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
		auto test = lights_[i.getId()];
		glm::mat4 Matrix = glm::mat4(1);
		Matrix = glm::translate(Matrix, i.getPosition());
		Matrix = glm::scale(Matrix, glm::vec3(i.getRange()));
		Lights newlight;
		newlight.Modelxform = Matrix;
		newlight.position = i.getPosition();
		newlight.intensity = i.getIntensity();
		newlight.range = i.getRange();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &newlight, GL_STREAM_DRAW);
		glBindVertexArray(light_sphere_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_sphere_mesh_.element_count, GL_UNSIGNED_INT, 0);
	}


	for (auto i : scene_->getAllSpotLights())
	{
		glBindBuffer(GL_UNIFORM_BUFFER, Light_BO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), &lights_[i.getId()], GL_STREAM_DRAW);
		glBindVertexArray(light_cone_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_cone_mesh_.element_count, GL_UNSIGNED_INT, 0);
	}
	glDepthMask(~0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glUseProgram(0);
}
void MyView::ShadowPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, Shadow_fbo);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glUseProgram(Shadow_Pre_Prog);

	
	// FIRST PASS
	for each (auto  light in lights_)
	{
		// only light that emit shadows
		if (light.second.shadows == true)
		{
			GLint Viewport[4];
			glGetIntegerv(GL_VIEWPORT, Viewport);
			auto aspect = Viewport[2] / (float)Viewport[3];
			auto camera = scene_->getCamera();
			//setup light view project
			/*auto view = glm::lookAt(camera.getPosition(), camera.getDirection(), scene_->getUpDirection());
			auto projection = glm::perspective(camera.getVerticalFieldOfViewInDegrees(), aspect, camera.getNearPlaneDistance(), camera.getFarPlaneDistance());*/
			auto view = glm::lookAt(light.second.position, -(light.second.position + light.second.direction), scene_->getUpDirection());
			auto projection = glm::ortho<float>(-100, 100, -100, 100, -100, 200);
			glUniformMatrix4fv(glGetUniformLocation(Shadow_Pre_Prog, "LightVP"), 1, false, glm::value_ptr(projection*view));
			//SetUniforms(Shadow_prog, view, projection);
			
			// draw scene
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, IBO);
			for (int i = 0; i < mesh_.size(); i++)
			{
				glBufferData(GL_ARRAY_BUFFER, InstanceData[i].size()*sizeof(Per_Instance), InstanceData[i].data(), GL_STREAM_DRAW);
				glDrawElementsInstancedBaseVertex(GL_TRIANGLES, mesh_[i].element_count, GL_UNSIGNED_INT, TGL_BUFFER_OFFSET(mesh_[i].element_offset), InstanceData[i].size(), mesh_[i].position_offset);
			}
		}
	}
	//second Pass
	glUseProgram(Shadow_prog);
	glUniform1i(glGetUniformLocation(Shadow_prog, "ShadowMap"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, Shadow_depth_tex);
	glDisable(GL_CULL_FACE);
	

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
	

	//for (auto i : scene_->getAllDirectionalLights())
	//{
	//	glm::mat4 Matrix = glm::mat4(1);
	//	Lights newlight;
	//	newlight.Modelxform = Matrix;
	//	newlight.intensity = i.getIntensity();
	//	newlight.direction = i.getDirection();
	//	
	//	lights_.insert(std::make_pair(i.getId(), newlight));
	//}

	//for (auto i : scene_->getAllDirectionalLights())
	//{
	//	glm::mat4 Matrix = glm::mat4(1);
	//	Lights newlight;
	//	newlight.Modelxform = Matrix;
	//	newlight.intensity = i.getIntensity();
	//	newlight.direction = i.getDirection();
	//	
	//	lights_.insert(std::make_pair(i.getId(), newlight));
	//}

	for (auto j : scene_->getAllPointLights())
	{
		glm::mat4 Matrix = glm::mat4(1);
		Matrix *= glm::translate(Matrix, j.getPosition());
		Matrix *= glm::scale(Matrix, glm::vec3(j.getRange()));
		Lights newlight;
		newlight.Modelxform = Matrix;
		newlight.position = j.getPosition();
		newlight.intensity = j.getIntensity();
		newlight.range = j.getRange();
		lights_.insert(std::make_pair(j.getId(), newlight));
	}
	for (auto i : scene_->getAllSpotLights())
	{
		glm::mat4 Matrix = glm::mat4(1);
		Matrix *= glm::translate(Matrix, i.getPosition());
		Matrix *= glm::scale(Matrix, glm::vec3(i.getConeAngleDegrees()/2, i.getConeAngleDegrees()/2,i.getRange()));
		Matrix *= glm::lookAt(i.getPosition(), i.getDirection(), scene_->getUpDirection());
		Lights newlight;
		newlight.Modelxform = Matrix;
		newlight.intensity = i.getIntensity();
		newlight.range = i.getRange();
		newlight.shadows = i.getCastShadow();
		newlight.position = i.getPosition();
		newlight.coneangledegrees = i.getConeAngleDegrees();
		newlight.direction = i.getDirection();
		lights_.insert(std::make_pair(i.getId(), newlight));
	}
}
void MyView::UpdateMeshs()
{
	InstanceData.clear();
	for (int i = 0; i < mesh_.size(); i++)
	{
		//if (i.isStatic())
		//	continue;
		std::vector<Per_Instance>NewInstace;
		for (auto j : scene_->getInstancesByMeshId(mesh_[i].id))
		{
			auto p = scene_->getInstanceById(j);
			Per_Instance newinst;
			auto mat = scene_->getMaterialById(p.getMaterialId());
			newinst.Model_xform = glm::mat4(p.getTransformationMatrix());
			newinst.diffuse = mat.getDiffuseColour();
			newinst.shininess = mat.getShininess();
			newinst.specular = mat.getSpecularColour();
			NewInstace.push_back(newinst);
		}
		InstanceData.push_back(NewInstace);
	}
}
bool MyView::CreateShader(GLuint& shader_program,
	std::string shader_name,
	std::vector<std::string> attriblocations,
	std::vector<std::string> fragdatalocations)
{
	GLint compile_status = 0;
	shader_program = glCreateProgram();
	GLuint vshader;
	vshader = glCreateShader(GL_VERTEX_SHADER); // create shader
	std::string vertex_shader_string = tygra::stringFromFile(shader_name+"_vs.glsl"); // load shader file
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vshader, 1, (const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetShaderInfoLog(vshader, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}

	compile_status = 0;
	GLuint fshader;
	fshader = glCreateShader(GL_FRAGMENT_SHADER); // create shader
	std::string frag_shader_string = tygra::stringFromFile(shader_name+"_fs.glsl"); // load shader file
	const char *frag_shader_code = frag_shader_string.c_str();
	glShaderSource(fshader, 1, (const GLchar **)&frag_shader_code, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetShaderInfoLog(fshader, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	// attach shaders to the program
	glAttachShader(shader_program, vshader);
	for (int i = 0; i < attriblocations.size(); i++)
		glBindAttribLocation(shader_program, i, attriblocations[i].data());
	glAttachShader(shader_program, fshader);
	for (int i = 0; i < fragdatalocations.size(); i++)
		glBindFragDataLocation(shader_program, i, fragdatalocations[i].data());

	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glLinkProgram(shader_program);
	compile_status = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_status);
	if (compile_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetProgramInfoLog(shader_program, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	return true;
}
void MyView::AAPass()
{
	AAEdgePass();
	AABlendPass(FrameCount);
	AANeighbourPass();
	if (FirstPass)
		AAResolvePass();
}
void MyView::AAEdgePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_edge_fbo);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(SMAA_Edgeprog);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SMAA_InputTex);
	glUniform1i(glGetUniformLocation(SMAA_Edgeprog, "Input_tex"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, SMAA_DepthTex);
	glUniform1i(glGetUniformLocation(SMAA_Edgeprog, "Depth_tex"), 1);
	// draw quad
	glBindVertexArray(light_quad_mesh_.vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MyView::AABlendPass(int SampleNum)
{
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_blend_fbo);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(SMAA_Blendprog);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SMAA_edgeTex);
	glUniform1i(glGetUniformLocation(SMAA_Blendprog, "edge_tex"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, SMAA_areaTex);
	glUniform1i(glGetUniformLocation(SMAA_Blendprog, "area_tex"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, SMAA_seachTex);
	glUniform1i(glGetUniformLocation(SMAA_Blendprog, "search_tex"), 2);
	glm::ivec4 indices;
	glm::vec2 camjitter;
	// draw quad
	if (SampleNum == 0)
	{
		camjitter = glm::vec2(0.25, -0.25);
		indices = glm::ivec4(1,1,1,0);
	}
	else if (SampleNum == 1)
	{
		camjitter = glm::vec2(-0.25, 0.25);
		indices = glm::ivec4(2, 2, 2, 0);
	}
	glUniform4iv(glGetUniformLocation(SMAA_Blendprog, "subSampleIndices"), 1, glm::value_ptr(indices));
	glUniform2fv(glGetUniformLocation(SMAA_Blendprog, "CamJitter"),1, glm::value_ptr(camjitter));
	glBindVertexArray(light_quad_mesh_.vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MyView::AANeighbourPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_fbo);
	glUseProgram(SMAA_Neighbourprog);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SMAA_InputTex);
	glUniform1i(glGetUniformLocation(SMAA_Neighbourprog, "Input_tex"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, SMAA_blendTex);
	glUniform1i(glGetUniformLocation(SMAA_Neighbourprog, "blend_tex"), 1);
	glEnable(GL_FRAMEBUFFER_SRGB);
	// draw quad
	glBindVertexArray(light_quad_mesh_.vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisable(GL_FRAMEBUFFER_SRGB);
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	glBindTexture(GL_TEXTURE_2D, SMAA_PrevTex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Viewport[2], Viewport[3]); // copy final scene depth to smaa input texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MyView::AAResolvePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, SMAA_fbo);
	glUseProgram(SMAA_Resolveprog);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SMAA_OutputTex);
	glUniform1i(glGetUniformLocation(SMAA_Resolveprog, "CurFrame"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, SMAA_PrevTex);
	glUniform1i(glGetUniformLocation(SMAA_Resolveprog, "PrevFrame"), 1);
	glBindVertexArray(light_quad_mesh_.vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
bool MyView::CreateShaderAA(GLuint& shader_program,
	std::string shader_name)
{
	std::string shaderdefine = { "#define SMAA_ONLY_COMPILE_VS 1\n" };
	std::string preDefine = { "#version 330 compatibility\n\
							  #ifndef SMAA_PIXEL_SIZE\n\
							  #define SMAA_PIXEL_SIZE vec2(1.0 / 1280.0, 1.0 / 720.0)\n\
							  #endif\n\#define SMAA_PRESET_ULTRA 1\n\
							  #define SMAA_PREDICATION 1\n\
							  #define SMAA_GLSL_3 1\n" };
	GLint compile_status = 0;
	shader_program = glCreateProgram();
	GLuint vshader;
	vshader = glCreateShader(GL_VERTEX_SHADER); // create shader
	std::string vertex_shader_string = preDefine+shaderdefine + tygra::stringFromFile("SMAA.h") + tygra::stringFromFile(shader_name + "_vs.glsl"); // load shader file
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vshader, 1, (const GLchar **)&vertex_shader_code, NULL);
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetShaderInfoLog(vshader, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	shaderdefine = { "#define SMAA_ONLY_COMPILE_PS 1\n" };
	compile_status = 0;
	GLuint fshader;
	fshader = glCreateShader(GL_FRAGMENT_SHADER); // create shader
	std::string frag_shader_string = preDefine + shaderdefine + tygra::stringFromFile("SMAA.h") + tygra::stringFromFile(shader_name + "_fs.glsl"); // load shader file
	const char *frag_shader_code = frag_shader_string.c_str();
	glShaderSource(fshader, 1, (const GLchar **)&frag_shader_code, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		glGetShaderInfoLog(fshader, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	//// attach shaders to the program
	glAttachShader(shader_program, vshader);
	glAttachShader(shader_program, fshader);
	glBindAttribLocation(shader_program, 0, "vertex_position");
	glBindFragDataLocation(shader_program, 0, "OutputTex");
	glBindFragDataLocation(shader_program, 1, "BlendTex");
	glBindFragDataLocation(shader_program, 2, "EdgeTex");
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glLinkProgram(shader_program);
	compile_status = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_status);
	if (compile_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "compile failed";
		std::cerr << log << std::endl;
		glGetProgramInfoLog(shader_program, string_length, NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	return true;
}