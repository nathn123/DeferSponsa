#version 330

mat4 model_xform;
uniform mat4 projection_xform;
uniform mat4 view_xform;
in vec2 vertex_position;


void main(void)
{


	gl_Position = vec4(vertex_position,0.0, 1.0);
}
