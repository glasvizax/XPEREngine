#version 460

layout (location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 light_volume_model;


void main() 
{
	gl_Position = projection * view * light_volume_model * vec4(a_pos, 1.0f);
}