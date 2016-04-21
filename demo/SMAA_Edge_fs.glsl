#version 330 compatibility
#ifndef SMAA_PIXEL_SIZE
#define SMAA_PIXEL_SIZE vec2(1.0 / 1280.0, 1.0 / 720.0)
#endif
#define SMAA_PRESET_ULTRA 1
#define SMAA_GLSL_2 1
#define SMAA_ONLY_COMPILE_PS 1
#include "SMAA.h"

uniform sampler2D Input_tex;
in vec2 texcoord;
in vec4 offset[3];
in vec4 dummy2;
void main()
{
  #if SMAA_PREDICATION == 1
    gl_FragColor = SMAAColorEdgeDetectionPS(texcoord, offset, Input_tex, depthTex);
  #else
    gl_FragColor = SMAAColorEdgeDetectionPS(texcoord, offset, Input_tex);
  #endif
}