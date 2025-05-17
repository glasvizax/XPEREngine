#version 460
#define MAX_TEXTURES_NUM 16

struct MaterialDS 
{
	sampler2D diffuse[MAX_TEXTURES_NUM/2];
	int diffuse_count;
	sampler2D specular[MAX_TEXTURES_NUM/2];
	int specular_count;
	float diffuse_blends[MAX_TEXTURES_NUM/2];
	float specular_blends[MAX_TEXTURES_NUM/2];
	sampler2D normal;
	sampler2D height;
	float shininess;
};

in VS_OUT
{
	vec3 normal;
	vec2 uv;
} fs_in;

uniform MaterialDS material;

out vec4 a_fragment_color;

void main()
{
	vec3 diffuse = vec3(0.0f);
	for(int i = 0; i < material.diffuse_count; ++i) 
	{
		diffuse += texture(material.diffuse[i], fs_in.uv).rgb * material.diffuse_blends[i];
	}
	float specular = 0.0f;
	for(int i = 0; i < material.specular_count; ++i) 
	{
		specular += texture(material.specular[i], fs_in.uv).r * material.specular_blends[i];
	}

	//TODO : lighting

	a_fragment_color = vec4(diffuse, 1.0f);
}
