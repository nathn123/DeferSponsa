#version 330 
uniform mat4 LightVP;

in mat4 model_xform;
in vec3 specular;
in float shininess;
in vec3 diffuse;


in vec3 vertex_position;
in vec3 vertex_normal;

void main(void)
{
	mat4 combined_xform = LightVP*model_xform;
	gl_Position = model_xform*vec4(vertex_position, 1.0);
}
