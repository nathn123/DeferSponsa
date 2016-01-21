#version 330


uniform vec3 ambient_light;

in ivec4 gl_FragCoord;
out vec3 finalcolour;

void main(void)
{

	//ambient pass
	//applied across whole scene so ...
	finalcolour = ambient_light;
}
