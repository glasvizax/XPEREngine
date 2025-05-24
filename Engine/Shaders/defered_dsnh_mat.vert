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
	vec2 uv;
	flat mat3 TBN;
} vs_out;

uniform mat4 model;

void main()
{
	vec4 _position = model * vec4(a_position, 1.0f);
	gl_Position = projection * view * _position;

	vs_out.uv = a_uv;
	vs_out.position = _position.xyz;

	mat3 normal_matrix = mat3(transpose(inverse(model)));
	vec3 N = normalize(normal_matrix * a_normal);
    vec3 T = normalize(normal_matrix * a_tangent);
    vec3 B = normalize(normal_matrix * a_bitangent);
	vs_out.TBN = mat3(T, B, N);
}

