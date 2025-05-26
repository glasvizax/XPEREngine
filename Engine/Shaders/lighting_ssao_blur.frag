#version 460 core

in VS_OUT
{
	vec2 uv;
} fs_in;

layout (binding = 11) uniform sampler2D ssao_base;

uniform int blur_iterations;

out float FragColor;

void main() 
{
	vec2 texel_size = 1.0f / textureSize(ssao_base, 0);
	float result = 0.0f;

	int half_iterations = blur_iterations / 2;

	for(float x = -half_iterations; x < half_iterations; ++x)
	{
		for(float y = -half_iterations; y < half_iterations; ++y)
		{
			vec2 offset = vec2(x, y) * texel_size;
			result += texture(ssao_base, fs_in.uv + offset).r;
		}
	}

	FragColor = result / 32.0f;
}