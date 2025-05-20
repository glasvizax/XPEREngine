#version 460 core
#define KERNEL_SIZE 64
#define NOISE_TEX_SIZE vec2(4, 4)

in VS_OUT
{
	vec2 uv;
} fs_in;

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

	vec3 frag_pos_world = texture(position_tex, fs_in.uv).xyz;
	if(frag_pos_world.x == 0.0f) 
	{
		FragColor = 1.0f; 
		return;
	} 

	vec3 frag_pos = (view * vec4(texture(position_tex, fs_in.uv).xyz, 1.0f)).xyz;
	vec3 normal = (view * vec4(texture(normal_shininess_tex, fs_in.uv).xyz, 0.0f)).xyz;
	vec3 random_vec = texture(ssao_noise_tex, fs_in.uv * max_noise_uv).xyz;

	vec3 tangent = normalize(random_vec - normal * dot(normal, random_vec));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float radius = 0.4f;
	float occlusion = 0.0f;
	float bias = 0.05f;
	for(int i = 0; i < KERNEL_SIZE; ++i) 
	{
		vec3 sample_pos = TBN * samples[i];
		sample_pos = frag_pos + sample_pos * radius;
		vec4 offset = vec4(sample_pos, 1.0f);
		offset = projection * offset;

		vec2 actual_tex_coords = clamp((offset.xy / offset.w) * 0.5 + 0.5, -0.10f, 1.1f);
		vec3 actual_depth_world = texture(position_tex, actual_tex_coords).xyz;

		if(actual_depth_world.x == 0.0f && actual_depth_world.y == 0.0f && actual_depth_world.z == 0.0f)
		{
			continue;
		}

		float actual_depth_view = (view * vec4(actual_depth_world, 1.0f)).z;

		float actual_to_frag_distance = abs(frag_pos.z - actual_depth_view);

		float range_check = clamp(mix(0.0f, 1.0f, radius / actual_to_frag_distance), 0.0f, 1.0f);

		occlusion += (actual_depth_view >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;  
	}

	float final = 1.0f - (occlusion/KERNEL_SIZE);  

	FragColor = pow(final, 2);

}