#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <OpenGL/gl.h>
#include <iostream>

bool TOGGLE_ANIMATION = false;
// RGB Color variables -> key_callback_RGB
bool SYMBOL = false;
float R_COLOR = 0.0f;
float G_COLOR = 0.0f;
float B_COLOR = 0.0f;
float STEP = 0.05f;

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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// TOGGLE ANIMATION
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		TOGGLE_ANIMATION = !TOGGLE_ANIMATION;

	// TOGGLE INCREMENT OR DECREMENT
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		SYMBOL = !SYMBOL;

	// Fermeture de la fenêtre
	if (key == GLFW_KEY_A && action == GLFW_PRESS) // A = Q car GLFW comprend le Querty
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Mode RGB progressif au click
	if (!TOGGLE_ANIMATION)
		if (key == GLFW_KEY_R && action == GLFW_REPEAT)
		{
			if (!SYMBOL)
				R_COLOR += STEP;
			else
				R_COLOR -= STEP;
		}
		else if (key == GLFW_KEY_G && action == GLFW_REPEAT)
		{
			if (!SYMBOL)
				G_COLOR += STEP;
			else
				R_COLOR -= STEP;
		}
		else if (key == GLFW_KEY_B && action == GLFW_REPEAT)
		{
			if (!SYMBOL)
				B_COLOR += STEP;
			else
				B_COLOR -= STEP;
		}

	glClearColor(R_COLOR, G_COLOR, B_COLOR, 1.0);
}

// au click de la souris
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	double xpos, ypos;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		glClearColor(xpos / static_cast<double>(WINDOW_WIDTH), ypos / static_cast<double>(WINDOW_HEIGHT), 0.0, 1.0);
	}
}

// au survol de la souris
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (TOGGLE_ANIMATION)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		glClearColor(xpos / 255, 0.0, ypos / 255, 1.0);
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
	// #ifdef __APPLE__
	// 	// We need to explicitly ask for a 3.3 context on Mac
	// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// #endif
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TD 01 Ex 04", nullptr, nullptr);
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

	// Resized window
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetWindowSizeCallback(window, onWindowResized);

	// Key pressed
	glfwSetKeyCallback(window, key_callback);
	// Mouse pressed
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Mouse moved
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Boucle principale => Ce qui se passe à l'écran
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
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