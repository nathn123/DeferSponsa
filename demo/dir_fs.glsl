#version 330


uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_diffuse;
uniform sampler2DRect sampler_world_specular;
uniform sampler2DRect sampler_world_shininess;
uniform mat4 projection_xform;
uniform mat4 view_xform;

uniform vec3 Intensity;
uniform vec3 Direction;

out vec4 fragment_colour;

void main(void)
{
	vec3 fragNor = texelFetch(sampler_world_normal,ivec2( gl_FragCoord.xy)).xyz;
	vec3 fragPos = texelFetch(sampler_world_position, ivec2(gl_FragCoord.xy)).xyz;
	vec3 diffuse = texelFetch(sampler_world_diffuse, ivec2(gl_FragCoord.xy)).xyz;
	vec3 Specular = texelFetch(sampler_world_specular, ivec2(gl_FragCoord.xy)).xyz;
	float shininess = texelFetch(sampler_world_shininess, ivec2(gl_FragCoord.xy)).x;
	// initials specular diffuse and ambient
	//vec3 Is = vec3(0.0);
	//vec3 Id = vec3(0.0);
	//vec3 Ia = vec3(0.0);

	//vec3 Ld = Intensity; 
	//;
	//vec3 final_colour = vec3(0.0);
	//float attenuation = 1.0f;
	//vec3 direction_to_light = normalize(-Direction);
	//if (shininess > 0.0)
	//{
	//	// specular intensity
	//	vec3 reflection = reflect(-direction_to_light, normalize(fragNor));
	//	vec3 surface_to_viewer = normalize(-fragPos);
	//	float dot_prod_specular = max(dot(reflection, surface_to_viewer), 0.0);
	//	Is = Specular * (dot_prod_specular); // final specular intensity
	//}
	//else
	//	Is = vec3(0.01);

	//// ambient intensity
	//Id = (Ld * diffuse)* attenuation; // final diffuse intensity
	//// final colour
	//final_colour = Id + Is;
	fragment_colour = vec4(diffuse+Intensity *max(0.0f,dot(-fragNor,Direction)), 0.0);
}
