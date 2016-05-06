


in vec2 vertex_position;
uniform vec2 CamJitter;
out vec2 texcoord;
out vec2 pixcoord;
out vec4 offset[3];
out vec4 dummy2;
void main()
{
	vec2 vertpos = vertex_position + CamJitter;
	texcoord = ((vertpos)+1) / 2;
  vec4 dummy1 = vec4(0);
  SMAABlendingWeightCalculationVS(dummy1, dummy2, texcoord, pixcoord, offset);
  gl_Position = vec4(vertpos, 0.0f, 1.0f);
}


