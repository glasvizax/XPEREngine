#version 460

layout (binding = 5) uniform samplerCube skybox_cubemap;

in VS_OUT
{
	vec3 cubemap_coords;
} fs_in;

out vec4 FragColor;

void main()
{
	FragColor = texture(skybox_cubemap, fs_in.cubemap_coords);
}