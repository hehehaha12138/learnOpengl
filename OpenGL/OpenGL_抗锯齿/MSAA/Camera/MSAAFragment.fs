#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS screenTexture;


void main()
{
	ivec2 texSize = textureSize(screenTexture);
	vec4 sum = vec4(0.0);

	for (int i = 0; i < 4; i++) {
		sum += texelFetch(screenTexture, ivec2(TexCoords*texSize), i);
	}
	FragColor = sum / 4;
}