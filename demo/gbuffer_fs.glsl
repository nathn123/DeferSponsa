#version 330


in vec3 varying_normal;
in vec3 varying_position;
in vec3 varying_diffuse;
in vec3 varying_specular;
in float varying_shininess;

layout(location = 0 ) out vec3 fragment_position;
layout(location = 1 ) out vec3 fragment_normal;
layout(location = 2 ) out vec3 fragment_diffuse;
layout(location = 3 ) out vec3 fragment_specular;
layout(location = 4 ) out float fragment_shininess;

void main(void)
{
	fragment_position =varying_position; // write position to texture
	fragment_normal = varying_normal; // write normal to texture
	fragment_diffuse = varying_diffuse;
    fragment_specular = varying_specular;
	fragment_shininess = varying_shininess;
}
