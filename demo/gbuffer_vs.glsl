#version 330

uniform mat4 projection_xform;
uniform mat4 view_xform;
uniform Instance
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
	varying_position = vec3(view_xform * model_xform * vec4(vertex_position, 1.0));
	varying_normal = vec3(view_xform * model_xform * vec4(vertex_normal, 0.0));
	varying_diffuse = diffuse;
    varying_specular = specular;
	varying_shininess = shininess;
	gl_Position = projection_xform*view_xform * model_xform *vec4(vertex_position, 1.0);

}
