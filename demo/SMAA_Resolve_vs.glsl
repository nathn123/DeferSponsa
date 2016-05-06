

in vec2 vertex_position;
out vec2 texcoord;
out vec4 dummy2;
void main()
{
	texcoord = ((vertex_position ) + 1) / 2;
  vec4 dummy1 = vec4(0);
  SMAAResolveVS(dummy1, dummy2, texcoord);
  gl_Position = vec4(vertex_position,0.0f, 1.0f);
}


