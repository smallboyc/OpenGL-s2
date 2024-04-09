#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <OpenGL/gl.h>
#include <iostream>

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 1.;

int const WINDOW_WIDTH = 800;
int const WINDOW_HEIGHT = 800;

static float aspectRatio;
void onWindowResized(GLFWwindow *window, int width, int height)
{
	aspectRatio = width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (aspectRatio > 1)
	{
		glOrtho(-GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio, -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -1.0, 1.0);
	}
	else
	{
		glOrtho(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio, -1.0, 1.0);
	}
}

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

int main()
{
	// Initialize the GLFW library
	if (!glfwInit())
	{
		return -1;
	}

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TD 01 Ex 03", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

		// Execute onWindowResized une première fois
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Quand la fenêtre se redimensionne, appelle onWindowResized -> Une callback intervient lors de l'interaction = addEventListener en js
	glfwSetWindowSizeCallback(window, onWindowResized);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1, 0.0, 0.0, 0.0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
		}
	}

	glfwTerminate();

	return 0;
}