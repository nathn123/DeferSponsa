#version 330


uniform vec3 light_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_position;
uniform float light_range;
uniform float light_intensity = 0.15;

uniform mat4 view_xform;
in ivec4 gl_FragCoord;

out vec3 reflected_light;

void main(void)
{
	vec3 fragNor = texelFetch(sampler_world_normal, gl_FragCoord.xy).xyz;
	vec3 fragPos = texelFetch(sampler_world_position, gl_FragCoord.xy).xyz;
	// specular intensity
	vec3 light_position_view = vec3(view_xform * vec4(light_position, 1.0)); // light position in view space
	vec3 direction_to_light = normalize(light_position_view - fragPos); // normalized direction to light
	float distance_to_light = distance(light_position_view, fragPos); // distance to light 
	vec3 surface_to_viewer = normalize(-fragPos);
	float dot_prod_specular = max(dot(reflection, surface_to_viewer), 0.0);
	float specular_factor = pow(dot_prod_specular, specular_exponent);
	final_attenuation = 1 - smoothstep(0, Range, distance_to_light);
	reflected_light = vec3(0.0, 0.33, 1.0) *(specular_factor*final_attenuation); // final specular intensity

	//float diffusefactor = dot(normalize(fragNor),-light_direction);
	//vec3 DiffuseColour = vec3(0);
	//if(diffusefactor >0)
	//	DiffuseColour = ambientcolour * light_intensity * diffusefactor;
	//reflected_light = DiffuseColour;
    //reflected_light = vec3(1.0, 0.33, 0.0);
}
