#version 460

layout (binding = 14) uniform sampler2D blurred_texture;

uniform bool horizontal;

in VS_OUT
{
	vec2 uv;
} fs_in;

out vec4 FragColor;

const float weight[5] = float[] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

void main() 
{	
	vec2 tex_offset = 1.0f / textureSize(blurred_texture, 0);
	vec3 result = texture(blurred_texture, fs_in.uv).rgb * weight[0];

	if(horizontal) 
	{
		for(int i = 1; i < 5; ++i) 
		{
			result += texture(blurred_texture, fs_in.uv + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
			result += texture(blurred_texture, fs_in.uv - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else 
	{
		for(int i = 1; i < 5; ++i) 
		{
			result += texture(blurred_texture, fs_in.uv + vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
			result += texture(blurred_texture, fs_in.uv - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0f);
}