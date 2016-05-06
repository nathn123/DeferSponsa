#version 330 
uniform mat4 LightMVP;


in mat4 model_xform;


in vec3 vertex_position;

out vec4 ShadowCoord;

void main(void)
{
	vec4 VertexModelSpace = model_xform * vec4(vertex_position,1.0);
	ShadowCoord = LightMVP* VertexModelSpace;
	gl_Position = LightMVP* VertexModelSpace;
}
