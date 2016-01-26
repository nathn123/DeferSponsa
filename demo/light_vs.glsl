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

out vec4 gl_Position;


void main(void)
{
	mat4 model_xform = mat4( 1.0 ); // identity
	if ( Range != 0) // i.e not dir 
	{
		mat4 translate = mat4(vec4( 1.0, 0.0, 0.0,  Position.x),
							vec4( 0.0,1.0, 0.0,  Position.y),
							vec4( 0.0, 0.0,1.0,  Position.z),
							vec4( 0.0, 0.0, 0.0, 1.0));
		mat4 scale = mat4(   Range, 0.0, 0.0, 0.0,
					    0.0,  Range, 0.0, 0.0,
					    0.0, 0.0,  Range, 0.0,
					    0.0, 0.0, 0.0,  Range);
		mat4 rotation = mat4( 1.0, 0.0, 0.0, 0.0,
						 0.0,1.0, 0.0, 0.0,
						 0.0, 0.0,1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
		if ( Coneangledegrees == 0)
		{
			mat4 rotx = mat4( 1.0, 0.0, 0.0, 0.0,
				 0.0, cos(Direction.x), -sin(Direction.x), 0.0,
				 0.0, sin(Direction.x), cos(Direction.x), 0.0,
				 0.0, 0.0, 0.0, 1.0);
			mat4 roty = mat4(  cos(Direction.y), 0.0, sin(Direction.y), 0.0,
				 0.0,1.0, 0.0, 0.0,
				 -sin(Direction.y), 0.0, cos(Direction.y), 0.0,
				 0.0, 0.0, 0.0, 1.0);
			mat4 rotz = mat4(  cos(Direction.z), -sin(Direction.z), 0.0, 0.0,
				 sin(Direction.z), cos(Direction.z), 0.0, 0.0,
				 0.0, 0.0,1.0, 0.0,
				 0.0, 0.0, 0.0, 1.0);
			rotation = rotz*roty*rotx;
		}

		model_xform = translate *rotation* scale;
			// change vertex pos based on light range
	}
	gl_Position =  projection_xform*view_xform * model_xform * vec4(vertex_position,1.0, 1.0f);
}