#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;
in vec2 vertex_position;
out vec3 varying_position;
uniform Lights
{
	vec3 Position;
	vec3 Intensity;
	vec3 Direction;
	float Range;
	float Coneangledegrees;
	bool Shadows;
};

void main(void)
{
	mat4 model_xform = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1, }; // identity
	if (Lights.Range != 0) // i.e not dir 
	{
	mat4 translate = { 1, 0, 0, Lights.Position.x,
						0, 1, 0, Lights.Position.y,
						0, 0, 1, Lights.Position.z,
						0, 0, 0, 1,};
	mat4 scale = { Light.Range, 0, 0, 0,
					0, Light.Range, 0, 0,
					0, 0, Light.Range, 0,
					0, 0, 0, Light.Range };
	mat4 rotation = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
	if (Lights.Coneangledegrees == 0)
	{
		mat4 rotx = { 1, 0, 0, 0,
			0, cos(direction.x), -sin(direction.x), 0,
			0, sin(direction.x), cos(direction.x), 0,
			0, 0, 0, 1 };
		mat4 roty = { cos(direction.y), 0, sin(direction.y), 0,
			0, 1, 0, 0,
			-sin(direction.y), 0, cos(direction.y), 0,
			0, 0, 0, 1 };
		mat4 rotz = { cos(direction.z), -sin(direction.z), 0, 0,
			sin(direction.z), cos(direction.z), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
		rotation = rotz*roty*rotx;
	}

	model_xform = translate *rotation* scale;
		// change vertex pos based on light range
	}
	gl_Position = projection_xform*view_xform * model_xform *vec4(vertex_position, 1.0);
