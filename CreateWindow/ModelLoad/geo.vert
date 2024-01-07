#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT{
	vec2 aTexCoord;
} vs_out;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0);
	vs_out.aTexCoord = aTexCoord;
}