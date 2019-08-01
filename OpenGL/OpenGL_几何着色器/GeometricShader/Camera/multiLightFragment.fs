#version 330 core

//����
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

//���Դ
struct PointLight {
	vec3 position;

	//˥������
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

//������Դֵԭ��
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

//Phong��ɫ
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;

	//���Դ
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	//��ɫ���
	FragColor = vec4(result, 1.0);

}

//���Դ��ɫ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	//���������
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

	//��������
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

	//���淴��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	//����˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	ambient *= attenuation;
	diffuse *= attenuation; 
	specular *= attenuation;

	//��ɫ���
	return(ambient + diffuse + specular);
}
