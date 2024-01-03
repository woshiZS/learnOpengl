#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;
void main()
{
	// FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

	// vec4 tempResult = texture(screenTexture, TexCoords);
	// float average = (0.2126 * tempResult.r + 0.7152 * tempResult.g + 0.0722 * tempResult.b) / 1.0;
	// FragColor = vec4(average, average, average, 1.0);

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0, offset),
		vec2(offset, offset),
		vec2(offset, 0),
		vec2(offset, -offset),
		vec2(0, -offset),
		vec2(-offset, -offset),
		vec2(-offset, 0),
		vec2(0, 0)
	);

	float kernel[9] = float[](
		1, 1, 1, 1, 1, 1, 1, 1, -8
	);

	vec3 sampleRes[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleRes[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	vec3 res = vec3(0.0);
	for(int i = 0; i < 9; ++i)
		res += sampleRes[i] * kernel[i] ;

	FragColor = vec4(res, 1.0);
}