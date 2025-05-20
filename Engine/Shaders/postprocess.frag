#version 460

in VS_OUT
{
	vec2 uv;
} fs_in;

layout (binding = 13) uniform sampler2D input_texture;
 
out vec4 a_fragment_color;

void main()
{
	vec3 color = texture(input_texture, fs_in.uv).rgb;
	a_fragment_color = vec4(color, 1.0f);
}
