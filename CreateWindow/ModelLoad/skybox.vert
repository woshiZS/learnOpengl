#version 330
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140) uniform Matrices{
	uniform mat4 projection;
};
uniform mat4 view;

void main()
{
	TexCoords = aPos;
	gl_Position = (projection * view * vec4(aPos, 1.0)).xyww;
}