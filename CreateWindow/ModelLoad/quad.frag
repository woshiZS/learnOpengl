#version 330 core

uniform sampler2D screenTexture;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	vec4 col = texture(screenTexture, TexCoords);
	float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
	FragColor = vec4(grayscale, grayscale, grayscale, 1.0);
}