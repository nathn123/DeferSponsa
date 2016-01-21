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

void main(void)
{
	gl_FragData[0] = varying_position; // write position to texture
	gl_FragData[1] = varying_normal; // write normal to texture
}
