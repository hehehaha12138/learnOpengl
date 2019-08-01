#version 330 core

//����
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float     shininess;
};

//��������
struct Light {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

//Phong��ɫ
void main()
{
	//����˥��ϵ��
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + 
		light.quadratic*(distance*distance));

	//���������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//��������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//���淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//�������ͼ
	vec3 emission = vec3(texture(material.emission, TexCoords));
	//��ɫ���
	vec3 result = attenuation*(ambient + diffuse + specular + emission);
	FragColor = vec4(result, 1.0);
}