#version 330


uniform vec3 ambient_light;

out vec4 finalcolour;

void main(void)
{

	//ambient pass
	//applied across whole scene so ...
	finalcolour = vec4(ambient_light,1.0);
}
