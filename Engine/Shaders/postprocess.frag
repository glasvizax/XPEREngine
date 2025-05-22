#version 460

layout (binding = 13) uniform sampler2D input_texture;
layout (binding = 14) uniform sampler2D input_bloom_texture;

in VS_OUT
{
	vec2 uv;
} fs_in;

out vec4 FragColor;

void main()
{
	float exposure = 1.0f;
	
	vec3 base_color = texture(input_texture, fs_in.uv).rgb;
	vec3 bloom_color = texture(input_bloom_texture, fs_in.uv).rgb;

	base_color += bloom_color;
	
	vec3 mapped = vec3(1.0f) - exp(-base_color * exposure);

	FragColor = vec4(mapped, 1.0f);
}
