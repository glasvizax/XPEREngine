#version 460

layout (location = 0) out vec4 a_postition;
layout (location = 1) out vec4 a_normal_shininess;
layout (location = 2) out vec4 a_diffuse_specular;

struct MaterialColor
{
	vec3 color;
	float shininess;
	float specular_scalar;
};

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

uniform MaterialColor material;

void main()
{
	vec3 color = material.color;
	a_postition = vec4(fs_in.position.xyz, 0.0f);
	a_diffuse_specular.rgb = color;
	a_diffuse_specular.a = material.specular_scalar;
	a_normal_shininess.xyz = fs_in.normal;
	a_normal_shininess.a = material.shininess;
}
