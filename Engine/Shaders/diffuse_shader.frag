#version 460
#define MAX_TEXTURES_NUM 16

struct MaterialD 
{
	sampler2D diffuse[MAX_TEXTURES_NUM];
	int diffuse_count;
	float diffuse_blends[MAX_TEXTURES_NUM];
	float shininess;
	float specular;
};

in VS_OUT
{
	vec3 normal;
	vec2 uv;
} fs_in;

uniform MaterialD material;

out vec4 a_fragment_color;

void main()
{
	vec3 color = vec3(0.0f);
	for(int i = 0; i < material.diffuse_count; ++i) 
	{
		color += texture(material.diffuse[i], fs_in.uv).rgb * material.diffuse_blends[i];
	}
	a_fragment_color = vec4(color, 1.0f);
}
