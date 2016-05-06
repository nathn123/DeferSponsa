//#version 330 compatibility
//#ifndef SMAA_PIXEL_SIZE
//#define SMAA_PIXEL_SIZE vec2(1.0 / 1280.0, 1.0 / 720.0)
//#endif
//#define SMAA_PRESET_ULTRA 1
//#define SMAA_GLSL_3 1
//#define SMAA_ONLY_COMPILE_VS 1
//#include "SMAA.h"

in vec2 vertex_position;
out vec2 texcoord;
out vec4 offset[2];
out vec4 dummy2;
void main()
{
	texcoord = (vertex_position + 1) / 2;;
  vec4 dummy1 = vec4(0);
  SMAANeighborhoodBlendingVS(dummy1, dummy2, texcoord, offset);
  gl_Position = vec4(vertex_position,0.0f, 1.0f);
}