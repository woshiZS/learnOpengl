#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 aNormal;
}gs_in[];

uniform mat4 view;
uniform mat4 projection;

const float magnitude = 0.4;

void GenerateLine(int index)
{
	gl_Position = projection * view * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * view * (gl_in[index].gl_Position + vec4(gs_in[index].aNormal, 0.0) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}