#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;

in vec3 vertex_position;
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

out vec3 varying_position;

out vec4 gl_Position;


void main(void)
{
	vec3 FinalPosition = vec3(0.0);
	//mat4 model_xform = mat4( 1.0 ); // identity
	//mat4 pre_model_xform;
	if ( Range != 0) // i.e not dir 
	{
		//mat4 translate = mat4(vec4( 1.0, 0.0, 0.0, Position.x),
		//					  vec4( 0.0, 1.0, 0.0, Position.y),
		//					  vec4( 0.0, 0.0, 1.0, Position.z),
		//					  vec4( 0.0, 0.0, 0.0, 1.0));

		//mat4 scale = mat4(1.0,   0.0,   0.0, 0.0,
		//			      0.0,   1.0,   0.0, 0.0,
		//			      0.0,   0.0,   1.0, 0.0,
		//			      0.0,   0.0,   0.0, 1/Range);
		//mat4 rotation = mat4(1.0);
		//if ( Coneangledegrees != 0)
		//{
		//	mat4 rotx = mat4( 1.0, 0.0,              0.0,               0.0,
		//					  0.0, cos(Direction.x), -sin(Direction.x), 0.0,
		//		              0.0, sin(Direction.x), cos(Direction.x),  0.0,
		//		              0.0, 0.0,              0.0,               1.0);

		//	mat4 roty = mat4(  cos(Direction.y),  0.0, sin(Direction.y), 0.0,
		//					   0.0,               1.0, 0.0,              0.0,
		//		               -sin(Direction.y), 0.0, cos(Direction.y), 0.0,
		//		               0.0,               0.0, 0.0,              1.0);

		//	mat4 rotz = mat4(  cos(Direction.z), -sin(Direction.z), 0.0, 0.0,
		//		               sin(Direction.z), cos(Direction.z),  0.0, 0.0,
		//		               0.0,              0.0,               1.0, 0.0,
		//		               0.0,              0.0,               0.0, 1.0);

		//	rotation = rotz*roty*rotx;
		//}
		//pre_model_xform = scale  *rotation;
		//model_xform = pre_model_xform* translate;
		//	// change vertex pos based on light range
			FinalPosition = vec3( projection_xform*view_xform * Modelxform * vec4(vertex_position, 1.0f));
	}
	else if(Range == 0)
	{
		FinalPosition = vertex_position;
	}
	gl_Position = vec4(FinalPosition,1.0f);
}