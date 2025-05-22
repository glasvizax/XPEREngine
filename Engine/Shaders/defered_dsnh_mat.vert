#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;

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
	vec3 tangent;
    vec3 bitangent;
} vs_out;

uniform mat4 model;

void main()
{
	vec4 _position = model * vec4(a_position, 1.0f);
	gl_Position = projection * view * _position;

	vs_out.uv = a_uv;
	vs_out.position = _position.xyz;

	mat3 normal_matrix = mat3(transpose(inverse(model)));
	vs_out.normal = normalize(normal_matrix * a_normal);
    vs_out.tangent = normalize(normal_matrix * a_tangent);
    vs_out.bitangent = normalize(normal_matrix * a_bitangent);
}

