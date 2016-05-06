


uniform sampler2D edge_tex;
uniform sampler2D area_tex;
uniform sampler2D search_tex;
uniform ivec4 subSampleIndices;
in vec2 texcoord;
in vec2 pixcoord;
in vec4 offset[3];
in vec4 dummy2;

void main()
{
	gl_FragColor = SMAABlendingWeightCalculationPS(texcoord, pixcoord, offset, edge_tex, area_tex, search_tex, ivec4(0));
}