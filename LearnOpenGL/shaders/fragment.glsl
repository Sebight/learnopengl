#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;

	float shininess;
};

struct Light {
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Point light data
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;


void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoord));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.texture_specular1, TexCoord));

	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	diffuse *= intensity;
	specular *= intensity;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
