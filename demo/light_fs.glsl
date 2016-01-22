#version 330


uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_world_position;

uniform Lights
{
	vec3 Position;
	vec3 Intensity;
	vec3 Direction;
	float Range;
	float Coneangledegrees;
	bool Shadows;
};

out vec3 finalcolour;

void main(void)
{
	vec3 fragNor = texelFetch(sampler_world_normal,ivec2( gl_FragCoord.xy)).xyz;
	vec3 fragPos = texelFetch(sampler_world_position, ivec2(gl_FragCoord.xy)).xyz;
	vec3 final_colour = vec3(0.0);
	if (Lights.Range != 0)
	{
		// initials specular diffuse and ambient
		vec3 Is = vec3(0.0);
		vec3 Id = vec3(0.0);
		vec3 Ia = vec3(0.0);

		vec3 final_colour = vec3(0.0);
		float final_attenuation = 0.0;
		float attenuation = 0.1;
		float linear_attenuation = 0.6;
		float quadratic_attenuation = 0.3;

		// fixed point light properties
		vec3 light_position = Position;
		vec3 light_direction = Direction;
		vec3 Ls = vec3(1.0, 1.0, 1.0); // white specular colour
		vec3 Ld = vec3(1.0, 1.0, 1.0); // dull white diffuse light colour

		//  // possible specular map conversion to vector

		// material
		vec3 Ks = vec3(0.0);
		if (Is_shiny == true)
			Ks = Specular;
		else
			Ks = vec3(1.0);
		vec3 Kd = diffuse; // apply material colour to material
		float specular_exponent = Range; // specular 'power'


		vec3 light_position_view = vec3(view_xform * vec4(light_position, 1.0)); // light position in view space
		vec3 direction_to_light = normalize(light_position_view - fragPos); // normalized direction to light
		float distance_to_light = distance(light_position_view, fragPos); // distance to light 
		float light_cosine = cos(Coneangledegrees / 2); // cosine of light angle

		//check if within light cone
		float light_cone = dot(normalize(light_direction), direction_to_light);// direction to light already normalized
		if (light_cone > light_cosine)
		{
			if (shininess > 0.0)
			{
				// specular intensity
				vec3 reflection = reflect(-direction_to_light, normalize(fragNor));
				vec3 surface_to_viewer = normalize(-fragPos);
				float dot_prod_specular = max(dot(reflection, surface_to_viewer), 0.0);
				float specular_factor = pow(dot_prod_specular, specular_exponent);
				//final_attenuation = 1.0 / (attenuation + linear_attenuation*distance_to_light + quadratic_attenuation * pow(distance_to_light, 2));
				final_attenuation = 1 - smoothstep(0, Range, distance_to_light);
				Is = Ks * (specular_factor*final_attenuation); // final specular intensity
			}
			else
				Is = vec3(0.01);
		}

		// ambient intensity
		Id = (Ld * Kd)* attenuation; // final diffuse intensity


		// final colour
		final_colour = Id + Is;
	}
	else
	{
		final_colour = Lights.Intensity;
	}

	fragment_colour += vec4(final_colour, 1.0);
}
