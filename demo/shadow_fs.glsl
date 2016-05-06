#version 330 

uniform sampler2D ShadowMap;

in vec4 ShadowCoord;
out vec4 fragment_colour;
void main()
{	
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
		
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
		
		
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
		
		
	float shadow = 1.0;
	if (ShadowCoord.w > 0.0)
	 	shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
	  	
		
	fragment_colour =	 vec4(0,0.0,shadow,0.0);
	  
}
		
