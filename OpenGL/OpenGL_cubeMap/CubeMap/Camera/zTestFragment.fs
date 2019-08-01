#version 330 core
out vec4 FragColor;


in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;
uniform samplerCube texture1;

void main()
{
	vec3 viewDir = normalize(cameraPos - Position);
	vec3 normal = normalize(Normal);

	vec3 R = reflect(-viewDir, normal);
	vec3 reflectMap = vec3(texture(texture_reflection1, TexCoords));
	vec3 reflection = vec3(texture(texture1, R).rgb)*reflectMap;

	float diff = max(normalize(dot(normal, viewDir)), 0.0f);
	vec3 diffuse = diff* vec3(texture(texture_diffuse1, TexCoords));
	
	FragColor = vec4(diffuse + reflection, 1.0f);
}
