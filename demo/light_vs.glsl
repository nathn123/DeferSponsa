#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;

in vec3 vertex_position;
uniform Lights
{
	mat4 Modelxform;
	vec3 Position;
	float Range;
	vec3 Intensity;
	float Coneangledegrees; 
	vec3 Direction;
	int Shadows;
};

out vec3 varying_position;

out vec4 gl_Position;


void main(void)
{
	gl_Position = projection_xform*view_xform * Modelxform * vec4(vertex_position, 1.0f);
}