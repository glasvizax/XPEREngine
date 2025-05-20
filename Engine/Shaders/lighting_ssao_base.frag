#version 460 core
#define KERNEL_SIZE 64

layout(binding = 7) uniform sampler2D position_tex;
layout(binding = 8) uniform sampler2D normal_shininess_tex;
layout(binding = 9) uniform sampler2D diffuse_specular_tex;
layout(binding = 10) uniform sampler2D ssao_noise_tex;

uniform vec3 samples[KERNEL_SIZE];

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out float FragColor;

void main()
{
	vec2 screen_size = textureSize(position_tex, 0);
	vec2 noise_size = textureSize(ssao_noise_tex, 0);

	vec2 max_noise_uv = screen_size / noise_size; 

	vec2 uv = gl_FragCoord.xy / screen_size;
	vec3 frag_pos_world = texture(position_tex, uv).xyz;

	if(frag_pos_world.x == 0.0f) 
	{
		FragColor = 1.0f; 
		return;
	} 

	vec3 frag_pos_view = (view * vec4(frag_pos_world, 1.0f)).xyz;
	vec3 normal_view = (view * vec4(texture(normal_shininess_tex, uv).xyz, 0.0f)).xyz;

	vec2 noise_uv = gl_FragCoord.xy / noise_size;
	vec3 random_vec = texture(ssao_noise_tex, noise_uv).xyz;

	vec3 tangent = normalize(random_vec - normal_view * dot(normal_view, random_vec));
	vec3 bitangent = cross(normal_view, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal_view);

	float occlusion = 0.0f;
	float radius     = 0.2f;

	for(int i = 0; i < KERNEL_SIZE; ++i) 
	{
		vec3 sample_view = frag_pos_view + TBN * samples[i] * radius;
		vec4 offset = projection * vec4(sample_view, 1.0f);

		vec2 actual_tex_coords = clamp((offset.xy / offset.w) * 0.5 + 0.5, 0.0f, 1.0f);
		vec3 actual_depth_world = texture(position_tex, actual_tex_coords).xyz;

		if(actual_depth_world.x == 0.0f && actual_depth_world.y == 0.0f && actual_depth_world.z == 0.0f)
		{
			continue;
		}

		vec3 to_sample = normalize(sample_view - frag_pos_view);

        float bias = max(0.015 * (1.0 - dot(normal_view, to_sample)), 0.005); 

		float actual_depth_view = (view * vec4(actual_depth_world, 1.0f)).z;

		float actual_to_frag_distance = abs(frag_pos_view.z - actual_depth_view);
		
		float range_check = smoothstep(0.1f, 0.9f, radius / actual_to_frag_distance);

		occlusion += (actual_depth_view >= sample_view.z + bias ? 1.0 : 0.0) * range_check;  
	}

	float final = 1.0f - (occlusion / KERNEL_SIZE);  

	FragColor = pow(final, 8);

}