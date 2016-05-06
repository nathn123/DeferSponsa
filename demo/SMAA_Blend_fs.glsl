


uniform sampler2D edge_tex;
uniform sampler2D area_tex;
uniform sampler2D search_tex;
in vec2 texcoord;
in vec2 pixcoord;
in vec4 offset[3];
in vec4 dummy2;
layout(location = 2 ) out vec4 EdgeTex;
layout(location = 1 ) out vec4 BlendTex;
layout(location = 0 ) out vec4 OutputTex;
void main()
{
  BlendTex = SMAABlendingWeightCalculationPS(texcoord, pixcoord, offset, edge_tex, area_tex, search_tex, ivec4(0));
}