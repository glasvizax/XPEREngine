#version 460 

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;

out VS_OUT
{
	vec2 uv;
} vs_out;

void main()
{
    gl_Position = vec4(a_pos, 0.0f, 1.0f);
	vs_out.uv = a_uv;
}
