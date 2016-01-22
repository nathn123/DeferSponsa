#version 330


uniform vec3 light_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_position;
uniform float light_range;
uniform float light_intensity = 0.15;

uniform mat4 view_xform;
in ivec4 gl_FragCoord;

in vec3 varying_normal;
in vec3 varying_position;

out vec3 fragment_position;
out vec3 fragment_normal;

void main(void)
{
	fragment_position = varying_position; // write position to texture
	fragment_normal = varying_normal; // write normal to texture
}
