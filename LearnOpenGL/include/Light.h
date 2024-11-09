class Light {
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

	glm::vec3 GetAmbient() const { return m_ambient; };
	glm::vec3 GetDiffuse() const { return m_diffuse; };
	glm::vec3 GetSpecular() const { return m_specular; };

protected:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

class PointLight : public Light {
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular),
	m_position(position),
	m_constant(constant),
	m_linear(linear),
	m_quadratic(quadratic) {}

	glm::vec3 GetPosition() const { return m_position; };
	float GetConstant() const { return m_constant; };
	float GetLinear() const { return m_linear; };
	float GetQuadratic() const { return m_quadratic; };
private:
	glm::vec3 m_position;
	float m_constant;
	float m_linear;
	float m_quadratic;
};

class SpotLight : public Light {
public:
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float radius) : Light(ambient, diffuse, specular),
		m_position(position),
		m_direction(direction),
		m_radius(radius) {}

	glm::vec3 GetPosition() const { return m_position; };
	glm::vec3 GetDirection() const { return m_direction; };
	float GetInnerCutoff() const { return glm::cos(glm::radians(m_radius)); };
	float GetOuterCutoff() const { return glm::cos(glm::radians(m_radius + 5)); };

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_radius;
};