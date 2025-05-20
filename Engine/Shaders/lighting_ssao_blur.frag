#version 460 core

in VS_OUT
{
	vec2 uv;
} fs_in;

layout (binding = 11) uniform sampler2D ssao_base;

out float FragColor;

void main() 
{
	vec2 texel_size = 1.0f / textureSize(ssao_base, 0);
	float result = 0.0f;

	for(float x = -2.0f; x < 2.0f; ++x)
	{
		for(float y = -2.0f; y < 2.0f; ++y)
		{
			vec2 offset = vec2(x, y) * texel_size;
			result += texture(ssao_base, fs_in.uv + offset).r;
		}
	}

	FragColor = result / 16.0f;
}