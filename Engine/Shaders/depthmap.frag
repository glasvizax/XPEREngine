#version 460

struct PointLight // 64
{
	vec3 position;
    float linear;
	vec3 ambient;
    float quadratic;
	vec3 diffuse;
    float _PAD0_;
	vec3 specular;
    float _PAD1_;
};

layout (std140, binding = 1) uniform Lights
{
	PointLight point_lights[100]; // 64 * 100
    int count;
};

uniform float depthmap_far;
uniform int current_light_index;

in vec3 g_pos;

void main()
{             
    vec3 pl_position = point_lights[current_light_index].position;
    gl_FragDepth = length(g_pos - pl_position) / depthmap_far;
}  