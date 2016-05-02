#version 330


uniform mat4 projection_xform;
uniform mat4 view_xform;
in vec2 vertex_position;
uniform mat4 Modelxform;

out vec4 gl_Position;
void main(void)
{
gl_Position = vec4(vertex_position,0.0f, 1.0f);
}
