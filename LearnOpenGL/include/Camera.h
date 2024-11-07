#pragma once
class Camera
{
public:
	Camera() = default;
	void OnInput(int key, bool press, float dt);
	void OnMouse(double xPos, double yPos);
	void OnScroll(double offset);

	glm::vec3 GetPosition() const;
	
	glm::mat4 GetView() const;
	float GetFov() const;
private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_yaw = -90.0f;
	float m_pitch = 0;
	bool m_firstMouse = true;
	const float m_cameraSpeed = 50.0f;
	bool m_shiftPressed = false;
	float m_lastX = 400;
	float m_lastY = 300;

	const float FOV_MIN = 1.0f;
	const float FOV_MAX = 45.0f;
	float m_fov = FOV_MAX;
};

