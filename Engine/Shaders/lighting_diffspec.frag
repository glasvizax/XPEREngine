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

layout(binding = 7) uniform sampler2D position_tex;
layout(binding = 8) uniform sampler2D normal_shininess_tex;
layout(binding = 9) uniform sampler2D diffuse_specular_tex;

uniform vec3 camera_position;
uniform int current_light_index;

out vec4 FragColor;

void main()
{
    vec2 tex_coords = gl_FragCoord.xy / textureSize(diffuse_specular_tex, 0);

    vec3 fragment_position = texture(position_tex, tex_coords).xyz;
    vec4 diffuse_specular = texture(diffuse_specular_tex, tex_coords);
    vec4 normal_shininess = texture(normal_shininess_tex, tex_coords);

    vec3 color = diffuse_specular.rgb;

    vec3 normal = normalize(normal_shininess.rgb);
    vec3 to_camera = normalize(camera_position - fragment_position);
    
    vec3 result = vec3(0.0); 

    // Diffuse
    vec3 to_light = point_lights[current_light_index].position - fragment_position;
    vec3 to_light_norm = normalize(to_light);
    float diff = max(dot(normal, to_light_norm), 0.0f);
    vec3 diffuse = point_lights[current_light_index].diffuse * diff;
        
    // Specular
    vec3 halfway = normalize(to_light_norm + to_camera);
    float spec = pow(max(dot(normal, halfway), 0.0f), normal_shininess.a);
    vec3 specular = point_lights[current_light_index].specular * spec * diffuse_specular.a;
        
    // Attenuation 
    float d = dot(to_light, to_light_norm);
	float attenuation = 1.0 / (1.0f + point_lights[current_light_index].linear * d + point_lights[current_light_index].quadratic * (d * d));   
        
    result += (diffuse + specular) * color * attenuation;

    FragColor = vec4(result, 1.0f);
}