#version 460

in VS_OUT
{
	vec2 tex_coords;
} fs_in;

uniform sampler2D tex;

out vec4 aFragColor;

void main()
{
	vec3 color = texture(tex, fs_in.tex_coords).rgb;
	aFragColor = vec4(color, 1.0f);
}
