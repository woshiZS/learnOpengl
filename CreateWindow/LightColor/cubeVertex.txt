#version 330 core
layout(location = 6) in vec3 aPos;
layout(location = 7) in vec3 aNorm;
layout(location = 8) in vec2 aTexcoord;

out vec3 norm;
out vec3 fragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	norm = mat3(transpose(inverse(model))) * aNorm;
	fragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexcoord;
}