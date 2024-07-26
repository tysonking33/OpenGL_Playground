#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../includes/stb_image.h"

#include "../includes/EBO.h"
#include "../includes/VAO.h"
#include "../includes/VBO.h"
#include "../includes/shaderClass.h"
#include "../includes/Texture.h"
#include "../includes/Camera.h"

void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	const unsigned int width = 800;
	const unsigned int height = 800;
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Construct the window
	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL Template", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
		return -1; // Exit the program if the window creation fails
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// Handle view port dimensions
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
								   { glViewport(0, 0, width, height); });

	GLfloat vertices[] =
		{
			-0.5f, 0.0f, 0.5f,	0.83f, 0.7f, 0.44f,		0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,	0.83f, 0.7f, 0.44f,		5.0f, 0.0f,
			0.5f, 0.0f, -0.5f,	0.83f, 0.7f, 0.44f,		0.0f, 0.0f,
			0.5f, 0.0f, 0.5f,	0.83f, 0.7f, 0.44f,		5.0f, 0.0f,
			0.0f, 0.8f, 0.0f,	0.92f, 0.86f, 0.76f,	2.5f, 5.0f
			};

	GLuint indices[] =
		{
			0,1,2,
			0,2,3,
			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4};

	Shader shaderProgram("../res/Shaders/default.vert", "../res/Shaders/default.frag");
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	Texture bean("../res/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	bean.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// This is the render loop
	while (!glfwWindowShouldClose(window))
	{

		// Druids are the best
		glClearColor(0.00f, 0.49f, 0.04f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		bean.Bind();
		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	bean.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
