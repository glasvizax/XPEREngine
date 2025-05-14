#version 460
#define MAX_TEXTURES_NUM 16

struct DiffuseTexture 
{
	int channels;
	sampler2D diffuse;
};

struct MaterialD 
{
	sampler2D diffuses[MAX_TEXTURES_NUM];
	int diffuses_num;
	float blend_coeffs[MAX_TEXTURES_NUM];
	float shininess;
	float specular;
};

struct MaterialDS
{
	sampler2D diffuses[MAX_TEXTURES_NUM/2];
	sampler2D speculars[MAX_TEXTURES_NUM/2];
	int diffuses_num;
	int specular_num;
	float diff_blend_coeffs[MAX_TEXTURES_NUM/2];
	float spec_blend_coeffs[MAX_TEXTURES_NUM/2];
	float shininess;
};

struct MaterialDSN
{
	sampler2D diffuses[MAX_TEXTURES_NUM/2 - 1];
	sampler2D speculars[MAX_TEXTURES_NUM/2 - 1];
	sampler2D normal;
	int diffuses_num;
	int specular_num;
	float diff_blend_coeffs[MAX_TEXTURES_NUM/2 - 1];
	float spec_blend_coeffs[MAX_TEXTURES_NUM/2 - 1];
	float shininess;
};

struct MaterialDSNH
{
	sampler2D diffuses[MAX_TEXTURES_NUM/2 - 1];
	sampler2D speculars[MAX_TEXTURES_NUM/2 - 1];
	sampler2D normal;
	sampler2D height;
	int diffuses_num;
	int specular_num;
	float diff_blend_coeffs[MAX_TEXTURES_NUM/2 - 1];
	float spec_blend_coeffs[MAX_TEXTURES_NUM/2 - 1];
	float shininess;
};

struct MaterialC 
{
	vec4 color;
	float shininess;
	float specular;
};

in VS_OUT
{
	vec3 normal;
	vec2 uv;
} fs_in;

uniform MaterialC material;

out vec4 a_fragment_color;

void main()
{
	vec3 color = material.color.rgb;
	a_fragment_color = vec4(color, 1.0f);
}
