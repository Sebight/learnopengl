#include "common.h"

#include "Shader.h"
#include "Camera.h"

#include "Vertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Light.h"

#include <filesystem>

typedef unsigned int uint;
constexpr int WINDOW_WIDTH = 1400;
constexpr int WINDOW_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Camera camera;
float lastTime = 0;
float dt = 0;

void processInput(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	camera.OnInput(GLFW_KEY_LEFT_SHIFT, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_W, glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_S, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_D, glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_A, glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_Q, glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS, dt);
	camera.OnInput(GLFW_KEY_E, glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS, dt);
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	camera.OnMouse(xPos, yPos);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.OnScroll(yOffset);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
	if (!window) {
		printf("Failed to create GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD.\n");
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders\\vertex.glsl", "shaders\\fragment.glsl");
	Shader lightShader("shaders\\vertex.glsl", "shaders\\light.glsl");
	
	const int pointsLightN = 2;
	PointLight pLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-100.0f, 50.0f, 0.0f), 1.0f, 0.007f, 0.0002f);
	PointLight pointLights[pointsLightN] = {
		pLight,
		PointLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(200.0f, 50.0f, 0.0f), 1.0f, 0.007f, 0.0002f)
	};

	const int spotLightsN = 2;
	SpotLight spotLights[spotLightsN] = {
		SpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 20),
		SpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(20.0f, 50.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 20)
	};

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//std::string path = "assets\\backpack\\backpack.obj";
	std::string path = "assets\\Sponza\\Sponza.gltf";
	Model myModel(path);
	path = "assets\\cube.obj";
	Model light(path);

	while (!glfwWindowShouldClose(window)) {
		float time = static_cast<float>(glfwGetTime());
		dt = time - lastTime;
		lastTime = time;
		// Input
		processInput(window, dt);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPos(0.0f, 80.0f, 0.0f);

		glm::mat4 proj = glm::perspective(glm::radians(camera.GetFov()), static_cast<float>(800) / static_cast<float>(600), 0.1f, 1000.0f);

		glm::mat4 view = camera.GetView();

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::scale(lightModel, glm::vec3(1.0f, 1.0f, 1.0f));
		lightModel = glm::translate(lightModel, lightPos);

		lightShader.Use();

		lightShader.SetMat4f("projection", proj);
		lightShader.SetMat4f("view", view);

		lightShader.SetMat4f("model", lightModel);

		light.Draw(lightShader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::translate(model, -lightPos);

		shader.Use();

		shader.SetMat4f("projection", proj);
		shader.SetMat4f("view", view);

		shader.SetMat4f("model", model);
		shader.SetVec3("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		shader.SetFloat("material.shininess", 32.0f);

		// Directional
		shader.SetVec3("dirLight.direction", 0.0f, -1.0f, -2.0f);
		shader.SetVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		shader.SetVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		// Point lights
		for (int i = 0; i < pointsLightN; i++) {
			// pointLights[id]
			char accessorBuffer[50];

			PointLight& l = pointLights[i];

			sprintf_s(accessorBuffer, 50, "pointLights[%d].position", i);
			shader.SetVec3(accessorBuffer, l.GetPosition().x, l.GetPosition().y, l.GetPosition().z);

			sprintf_s(accessorBuffer, 50, "pointLights[%d].constant", i);
			shader.SetFloat(accessorBuffer, l.GetConstant());

			sprintf_s(accessorBuffer, 50, "pointLights[%d].quadratic", i);
			shader.SetFloat(accessorBuffer, l.GetQuadratic());

			sprintf_s(accessorBuffer, 50, "pointLights[%d].linear", i);
			shader.SetFloat(accessorBuffer, l.GetLinear());

			sprintf_s(accessorBuffer, 50, "pointLights[%d].ambient", i);
			shader.SetVec3(accessorBuffer, l.GetAmbient().x, l.GetAmbient().y, l.GetAmbient().z);

			sprintf_s(accessorBuffer, 50, "pointLights[%d].diffuse", i);
			shader.SetVec3(accessorBuffer, l.GetDiffuse().x, l.GetDiffuse().y, l.GetDiffuse().z);

			sprintf_s(accessorBuffer, 50, "pointLights[%d].specular", i);
			shader.SetVec3(accessorBuffer, l.GetSpecular().x, l.GetSpecular().y, l.GetSpecular().z);
		}

		// Point lights
		for (int i = 0; i < spotLightsN; i++) {
			// pointLights[id]
			char accessorBuffer[50];

			SpotLight& l = spotLights[i];

			sprintf_s(accessorBuffer, 50, "spotLights[%d].position", i);
			shader.SetVec3(accessorBuffer, l.GetPosition().x, l.GetPosition().y, l.GetPosition().z);

			sprintf_s(accessorBuffer, 50, "spotLights[%d].direction", i);
			shader.SetVec3(accessorBuffer, l.GetDirection().x, l.GetDirection().y, l.GetDirection().z);

			sprintf_s(accessorBuffer, 50, "spotLights[%d].cutOff", i);
			shader.SetFloat(accessorBuffer, l.GetInnerCutoff());

			sprintf_s(accessorBuffer, 50, "spotLights[%d].outerCutOff", i);
			shader.SetFloat(accessorBuffer, l.GetOuterCutoff());

			sprintf_s(accessorBuffer, 50, "spotLights[%d].ambient", i);
			shader.SetVec3(accessorBuffer, l.GetAmbient().x, l.GetAmbient().y, l.GetAmbient().z);

			sprintf_s(accessorBuffer, 50, "spotLights[%d].diffuse", i);
			shader.SetVec3(accessorBuffer, l.GetDiffuse().x, l.GetDiffuse().y, l.GetDiffuse().z);

			sprintf_s(accessorBuffer, 50, "spotLights[%d].specular", i);
			shader.SetVec3(accessorBuffer, l.GetSpecular().x, l.GetSpecular().y, l.GetSpecular().z);
		}

		//// Spotlight
		//shader.SetVec3("spotLight.position", lightPos.x, lightPos.y, lightPos.z);
		//shader.SetVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
		//shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		//shader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//shader.SetVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
		//shader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

		myModel.Draw(shader);

		// Screen render
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}