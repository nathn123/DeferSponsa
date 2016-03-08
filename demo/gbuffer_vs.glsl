#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;
uniform mat4 combined_xform;
layout(std140) uniform Instance
{
	mat4 model_xform;
	vec3 specular;
	float shininess;
	vec3 diffuse;
};

in vec3 vertex_position;
in vec3 vertex_normal;


out vec3 varying_normal;
out vec3 varying_position;
out vec3 varying_diffuse;
out vec3 varying_specular;
out float varying_shininess;

void main(void)
{
	vec4 position4 = vec4(vertex_position.x,vertex_position.y,vertex_position.z, 1.0);
	//mat4 combined_xform = projection_xform*view_xform*model_xform;
	vec4 Pposition = projection_xform* vec4(vertex_position, 1.0);
	vec4 Pnormal = projection_xform * vec4(vertex_normal, 0.0);
	vec4 Vposition = view_xform* vec4(vertex_position, 1.0);
	vec4 Vnormal = view_xform * vec4(vertex_normal, 0.0);
	vec4 Mposition = model_xform* vec4(vertex_position, 1.0);
	vec4 Mnormal = model_xform * vec4(vertex_normal, 0.0);
	vec4 position = combined_xform* vec4(vertex_position, 1.0);
	vec4 normal = combined_xform * vec4(vertex_normal, 0.0);
	varying_position = vec3(position);
	varying_normal = vec3(normal);
	varying_diffuse = diffuse;
    varying_specular = specular;
	varying_shininess = shininess;
	gl_Position = model_xform *vec4(vertex_position, 1.0);

}
