//#version 330 compatibility
//#ifndef SMAA_PIXEL_SIZE
//#define SMAA_PIXEL_SIZE vec2(1.0 / 1280.0, 1.0 / 720.0)
//#endif
//#define SMAA_PRESET_ULTRA 1
//#define SMAA_GLSL_3 1
//#define SMAA_ONLY_COMPILE_VS 1
//#include "SMAA.h"

out vec2 texcoord;
out vec4 offset[3];
out vec4 dummy2;
void main()
{
  texcoord = gl_MultiTexCoord0.xy;
  vec4 dummy1 = vec4(0);
  SMAAEdgeDetectionVS(dummy1, dummy2, texcoord, offset);
  gl_Position = ftransform();
}