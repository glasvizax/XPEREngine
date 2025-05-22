#version 460

layout (location = 0) in vec3 a_pos;

out VS_OUT
{
	vec3 cubemap_coords;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 skybox_view;

void main()
{
	vs_out.cubemap_coords = a_pos;
	vec4 pos = projection * skybox_view * vec4(a_pos, 1.0f);
	gl_Position = pos.xyww;
}