#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;

	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

float ambientStrength = 0.1f;
float specularStrength = 0.5f;

void main()
{
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.texture_specular1, TexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
