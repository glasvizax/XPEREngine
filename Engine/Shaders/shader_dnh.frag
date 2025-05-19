#version 460

layout (location = 0) out vec4 a_postition;
layout (location = 1) out vec4 a_normal_shininess;
layout (location = 2) out vec4 a_diffuse_specular;

struct MaterialDNH 
{
	sampler2D diffuse;
	sampler2D normal;
	sampler2D height;
	float shininess;
	float specular_scalar;
};

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

uniform MaterialDNH material;

out vec4 a_fragment_color;

void main()
{
	vec3 color = texture(material.diffuse, fs_in.uv).rgb;

	a_postition = vec4(fs_in.position.xyz, 0.0f);
	a_diffuse_specular.rgb = color;
	a_diffuse_specular.a = material.specular_scalar;
	a_normal_shininess.xyz = fs_in.normal; //TODO: REAL NORMALS + HEIGHT
	a_normal_shininess.a = material.shininess;
}
