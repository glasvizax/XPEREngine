#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
//TODO : TANGENT BITANGENT

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;

uniform mat4 model;

void main()
{
	vec4 _position = model * vec4(a_position, 1.0f);
	gl_Position = projection * view * _position;

	vs_out.uv = a_uv;
	vs_out.normal = mat3(transpose(inverse(model))) * a_normal;
	vs_out.position = _position.xyz;
}

