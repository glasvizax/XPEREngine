#version 460

layout (binding = 13) uniform sampler2D input_texture;

uniform float threshold;  

in VS_OUT
{
	vec2 uv;
} fs_in;

out vec4 FragColor;

void main()
{
	vec3 color = texture(input_texture, fs_in.uv).rgb;
    
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); 

    if (brightness > threshold)
    {
        FragColor = vec4(color, 1.0);
    } 
    else
    {
        FragColor = vec4(vec3(0.0), 1.0f); 
    }
}
