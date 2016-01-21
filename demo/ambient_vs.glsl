#version 330

uniform mat4 model_xform;
in vec2 vertex_position;
out vec3 varying_position;

void main(void)
{

	varying_position = vec3(view_xform * model_xform * vec4(vertex_position, 1.0));
	vec4 model_position = model_xform * vec4(vertex_position,1.0);
	gl_Position = vec4(model_position);
}
