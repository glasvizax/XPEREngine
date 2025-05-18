#version 460
#define MAX_TEXTURES_NUM 16

struct MaterialDS 
{
	sampler2D diffuse;
	sampler2D specular;
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
	vec3 color = texture(material.diffuse, fs_in.uv).rgb;

	if(color.r > 0.98f && color.g > 0.98f && color.b > 0.98f) 
	{
		discard;
	}

	a_fragment_color = vec4(color, 1.0f);
}
