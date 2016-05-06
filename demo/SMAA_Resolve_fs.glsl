


uniform sampler2D CurFrame;
uniform sampler2D PrevFrame;

uniform ivec4 subSampleIndices;
in vec2 texcoord;
in vec2 pixcoord;
in vec4 offset[3];
in vec4 dummy2;

void main()
{
#if SMAA_REPROJECTION == 1
	gl_FragColor = SMAAResolvePS(texcoord,  CurFrame,  PrevFrame,  velocityTex);
#else
	gl_FragColor = SMAAResolvePS(texcoord, CurFrame, PrevFrame);

		
#endif
		
}