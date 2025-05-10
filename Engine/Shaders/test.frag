#version 460

in VS_OUT
{
	vec3 color;
} fs_in;

out vec4 aFragColor;

void main()
{
	aFragColor = vec4(fs_in.color, 1.0f);
}
