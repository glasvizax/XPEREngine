#version 460

struct MaterialColor
{
	vec3 color;
};

uniform MaterialColor material;

out vec4 FragColor;

void main()
{
	FragColor = vec4(material.color * 32, 1.0f);
}
