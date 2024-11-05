#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW3/glfw3.h>
#include "Camera.h"

void Camera::OnInput(int key, bool press, float dt)
{
	if (key == GLFW_KEY_LEFT_SHIFT) {
		m_shiftPressed = press;
	}
	float adjustedCameraSpeed = (m_shiftPressed ? m_cameraSpeed * 2 : m_cameraSpeed) * dt;

	if (key == GLFW_KEY_W && press) {
		m_cameraPos += adjustedCameraSpeed * m_cameraFront;
	}
	if (key == GLFW_KEY_S && press) {
		m_cameraPos -= adjustedCameraSpeed * m_cameraFront;
	}
	if (key == GLFW_KEY_A && press) {
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * adjustedCameraSpeed;
	}
	if (key == GLFW_KEY_D && press) {
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * adjustedCameraSpeed;
	}
}

void Camera::OnMouse(double xPos, double yPos)
{
	if (m_firstMouse) {
		m_lastX = static_cast<float>(xPos);
		m_lastY = static_cast<float>(yPos);
		m_firstMouse = false;
	}

	float xOffset = static_cast<float>(xPos) - m_lastX;
	float yOffset = static_cast<float>(yPos) - m_lastY;

	m_lastX = static_cast<float>(xPos);
	m_lastY = static_cast<float>(yPos);

	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_yaw += xOffset;
	m_pitch -= yOffset;

	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_cameraFront = glm::normalize(direction);
}

void Camera::OnScroll(double offset)
{
	m_fov -= static_cast<float>(offset);

	if (m_fov < FOV_MIN) {
		m_fov = FOV_MIN;
	}
	else if (m_fov > FOV_MAX) {
		m_fov = FOV_MAX;
	}
}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

float Camera::GetFov() const
{
	return m_fov;
}
