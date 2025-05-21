#version 460
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // maybe just triangle

uniform mat4 shadow_tranforms[6];

out vec3 g_pos;

void main() 
{	
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for (int i = 0; i < 3; ++i) 
		{
			g_pos = gl_in[i].gl_Position.xyz;
			gl_Position = shadow_tranforms[face] * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}