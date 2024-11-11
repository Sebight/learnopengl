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

void renderQuad();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Camera camera;
float lastTime = 0;
float dt = 0;

Shader shader;
Shader lightShader;
Shader depthShader;
Shader debugDepthQuad;

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

Model myModel;
Model light;

glm::vec3 lightPos(0.0f, 5.0f, -10.0f);
glm::mat4 lightProj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 200.0f);
glm::mat4 lightView = glm::lookAt(glm::vec3(2.0f, 4.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

uint shadowMap;


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

void RenderScene(bool projOverride = false, glm::mat4 projOverrideValue = glm::mat4(0.0f), bool viewOverride = false, glm::mat4 viewOverrideValue = glm::mat4(0.0f)) {
	// Render
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 proj = projOverride ? projOverrideValue : glm::perspective(glm::radians(camera.GetFov()), static_cast<float>(800) / static_cast<float>(600), 0.1f, 1000.0f);

	glm::mat4 view = viewOverride ? viewOverrideValue : camera.GetView();

	glm::mat4 model = glm::mat4(1.0f);

	//lightShader.Use();
	//lightShader.SetMat4f("projection", proj);
	//lightShader.SetMat4f("view", view);
	//lightShader.SetMat4f("model", model);

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

	shader.SetMat4f("lightSpaceMatrix", lightProj * lightView);

	light.Draw(shader);

	model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
	shader.SetMat4f("model", model);

	shader.SetInt("shadowMap", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	light.Draw(shader);

	return;

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

	myModel.Draw(shader);
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader = Shader("shaders\\vertex.glsl", "shaders\\fragment.glsl");
	lightShader = Shader("shaders\\vertex.glsl", "shaders\\light.glsl");
	myModel = Model("assets\\Sponza\\Sponza.gltf");
	light = Model("assets\\cube\\Cube.gltf");
	depthShader = Shader("shaders\\depthShader.glsl", "shaders\\depthShaderF.glsl");
	debugDepthQuad = Shader("shaders\\3.1.1.debug_quad.vs", "shaders\\3.1.1.debug_quad_depth.fs");

	uint shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	const uint SHADOW_MAP_WIDTH = 2048;
	const uint SHADOW_MAP_HEIGHT = 2048;

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glm::mat4 proj = glm::perspective(glm::radians(camera.GetFov()), static_cast<float>(800) / static_cast<float>(600), 0.1f, 1000.0f);
	glm::mat4 view = camera.GetView();

	glm::mat4 lightSpaceMatrix = lightProj * lightView;

	while (!glfwWindowShouldClose(window)) {
		float time = static_cast<float>(glfwGetTime());
		dt = time - lastTime;
		lastTime = time;
		// Input
		processInput(window, dt);

#if 1
		// Shadow pass
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

		glClear(GL_DEPTH_BUFFER_BIT);
		glm::mat4 model = glm::mat4(1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		depthShader.Use();
		depthShader.SetMat4f("lightSpaceMatrix", lightSpaceMatrix);
		depthShader.SetMat4f("model", model);
		light.Draw(depthShader);
		
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
		depthShader.SetMat4f("model", model);
		light.Draw(depthShader);

		//RenderScene(true, lightProj, true, lightView);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

#if 1
		// Quad debug
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		debugDepthQuad.Use();
		debugDepthQuad.SetFloat("near_plane", 0.1f);
		debugDepthQuad.SetFloat("far_plane", 750.0f);
		debugDepthQuad.SetInt("depthMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		renderQuad();
#endif

#if 1
		// Lighting pass
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene();
		//RenderScene(true, lightProj, true, lightView);
#endif

		// Screen render
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

// DEBUG
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}