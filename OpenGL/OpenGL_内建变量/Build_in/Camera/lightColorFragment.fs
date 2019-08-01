#version 330 core
out vec4 FragColor;
in vec3 LightColor;

uniform vec3 objectColor;


void main()
{
	//着色结果
	vec3 result = LightColor*objectColor;
	FragColor = vec4(result, 1.0);
}