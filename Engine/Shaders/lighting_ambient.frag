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
	PointLight point_lights[32]; // 64 * 100
    int count;
};

in VS_OUT
{
	vec2 uv;
} fs_in;

layout(binding = 7) uniform sampler2D position;
layout(binding = 8) uniform sampler2D normal_shininess;
layout(binding = 9) uniform sampler2D diffuse_specular;
layout(binding = 12) uniform sampler2D ssao_blur;

uniform float ambient_factor; 

out vec4 FragColor;

void main()
{
    vec4 diffuse_specular = texture(diffuse_specular, fs_in.uv);

    float ambient_occlusion = texture(ssao_blur, fs_in.uv).r;

    vec3 color = diffuse_specular.rgb * ambient_factor;
    vec3 frag_ambient = vec3(0.0); 
    for (int i = 0; i < count; ++i)
    {
        // Ambient
        vec3 ambient = point_lights[i].ambient * ambient_occlusion;
        frag_ambient += ambient * color;
    }

    FragColor = vec4(frag_ambient, 1.0f);
}

