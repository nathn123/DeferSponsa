#version 330


uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_diffuse;
uniform sampler2DRect sampler_world_specular;
uniform sampler2DRect sampler_world_shininess;
uniform mat4 projection_xform;
uniform mat4 view_xform;

uniform Lights
{
	mat4 Modelxform;
	vec3 Position;
	float Range;
	vec3 Intensity;
	float Coneangledegrees; 
	vec3 Direction;
	int Shadows;
};

out vec4 fragment_colour;

void main(void)
{
	vec3 fragNor = texelFetch(sampler_world_normal,ivec2( gl_FragCoord.xy)).xyz;
	vec3 fragPos = texelFetch(sampler_world_position, ivec2(gl_FragCoord.xy)).xyz;
	vec3 diffuse = texelFetch(sampler_world_diffuse, ivec2(gl_FragCoord.xy)).xyz;
	vec3 Specular = texelFetch(sampler_world_specular, ivec2(gl_FragCoord.xy)).xyz;
	float shininess = texelFetch(sampler_world_shininess, ivec2(gl_FragCoord.xy)).x;
	// initials specular diffuse and ambient
	vec3 Is = vec3(0.0);
	vec3 Id = vec3(0.0);
	vec3 Ia = vec3(0.0);

	vec3 Ls = Intensity;
	vec3 Ld = Intensity; 

	vec3 final_colour = vec3(0.0);
	float attenuation = 0.0;
	vec3 direction_to_light = vec3(0.0);	
	//float attenuation = 0.1;
	//float linear_attenuation = 0.6;
	//float quadratic_attenuation = 0.3;
	vec3 light_position_view = vec3(view_xform * vec4(Position, 1.0)); // light position in view space
	direction_to_light = normalize(light_position_view - fragPos); // normalized direction to light

	float distance_to_light = distance(light_position_view, fragPos); // distance to light 
	attenuation = 1 - smoothstep(0, Range, distance_to_light);
	// fixed point light properties
	vec3 light_position = Position;
	vec3 light_direction = Direction;



	if(Coneangledegrees != 0)
	{
		float light_cosine = cos(Coneangledegrees / 2); // cosine of light angle
		//check if within light cone
		float light_cone = dot(normalize(light_direction), direction_to_light);// direction to light already normalized
		if (light_cone < light_cosine)
			return; // break out
	}
	float specular_exponent = Range; // specular 'power'
	if (shininess > 0.0)
	{
		// specular intensity
		vec3 reflection = reflect(-direction_to_light, normalize(fragNor));
		vec3 surface_to_viewer = normalize(-fragPos);
		float dot_prod_specular = max(dot(reflection, surface_to_viewer), 0.0);
		float specular_factor = pow(dot_prod_specular, specular_exponent);
		Is = Specular * (specular_factor*attenuation); // final specular intensity
	}
	else
		Is = vec3(0.01);

	// ambient intensity
	Id = (Ld * diffuse)* attenuation; // final diffuse intensity
	// final colour
	final_colour = Id + Is;
	fragment_colour += vec4(final_colour, 1.0);
}
