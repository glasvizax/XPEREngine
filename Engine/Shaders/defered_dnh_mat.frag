#version 460

layout (location = 0) out vec4 a_postition;
layout (location = 1) out vec4 a_normal_shininess;
layout (location = 2) out vec4 a_diffuse_specular;

struct MaterialDN
{
	sampler2D diffuse;
	sampler2D normal;
	float shininess;
	float specular_scalar;
};

in VS_OUT
{
    vec3 position;
    vec2 uv;
    flat mat3 TBN;
} fs_in;

uniform MaterialDN material;

void main()
{
	vec3 color = texture(material.diffuse, fs_in.uv).rgb;

	vec3 sampled_normal = texture(material.normal, fs_in.uv).xyz * 2.0 - 1.0;
	vec3 final_normal = normalize(fs_in.TBN * sampled_normal);

	a_postition = vec4(fs_in.position.xyz, 0.0f);
	a_diffuse_specular.rgb = color;
	a_diffuse_specular.a = material.specular_scalar;
	a_normal_shininess.xyz = final_normal; //TODO: REAL NORMALS
	a_normal_shininess.a = material.shininess;
}
