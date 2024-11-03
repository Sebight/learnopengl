#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <iostream>
#include "Shader.h"

typedef unsigned int uint;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

float vertices[] = {
	// Pos			    // Color
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left
	0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // Top
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Right
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2,   // first triangle
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// VAO
	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO bind
	uint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	uint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("shaders\\vertex.glsl", "shaders\\fragment.glsl");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float offset = 0.0f;
	int dir = 1;

	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (offset > 1.0f) {
			dir = -1;
		}
		else if (offset < -1.0f) {
			dir = 1;
		}
		offset += 0.01f * dir;


		shader.Use();
		shader.SetFloat("offset", offset);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// Screen render
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}