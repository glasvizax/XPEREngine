#version 460
#define MAX_TEXTURES_NUM 16

struct DiffuseTexture 
{
	int channels;
	sampler2D diffuse;
};

struct MaterialColor
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

uniform MaterialColor material;

out vec4 a_fragment_color;

void main()
{
	vec3 color = material.color.rgb;
	a_fragment_color = vec4(color, 1.0f);
}
