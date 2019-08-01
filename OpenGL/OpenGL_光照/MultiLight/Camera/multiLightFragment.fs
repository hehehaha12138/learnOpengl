#version 330 core

//����
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float     shininess;
};

//�����
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

//���㶨���ֵԭ��
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

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
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

//������Դֵԭ��
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//�۹�Դ
struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;

//Phong��ɫ
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//�������
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//���Դ
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	//�۹�Դ
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	//��ɫ���
	FragColor = vec4(result, 1.0);

}

//�����(ƽ�й�)��ɫ
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);

	//���������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//��������
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//���淴��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//�������ͼ
	vec3 emission = vec3(texture(material.emission, TexCoords));
	//��ɫ���
	 return(ambient + diffuse + specular + emission);
}

//���Դ��ɫ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);

	//���������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//��������
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//���淴��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//�������ͼ
	vec3 emission = vec3(texture(material.emission, TexCoords));

	//����˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	ambient *= attenuation;
	diffuse *= attenuation; 
	specular *= attenuation;
	emission *= attenuation;

	//��ɫ���
	return(ambient + diffuse + specular + emission);
}

//�۹����ɫ
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	//��������
	float diff = max(dot(normal, lightDir), 0.0);

	//���淴��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	//�жϾ۹ⷶΧ
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	//����˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//�������ͼ
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	vec3 emission = vec3(texture(material.emission, TexCoords));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	emission *= attenuation * intensity;

	return(ambient + diffuse + specular + emission);

	
}