#define GLFW_INCLUDE_NONE
#define _USE_MATH_DEFINES

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "3D_tools.hpp"
#include "draw_scene.hpp"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "TD05";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
static int flag_filaire = 0;
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /* window */, int width, int height)
{
	aspectRatio = width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setPerspective(60.0, aspectRatio, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void onKey(GLFWwindow *window, int key, int /* scancode */, int action, int /* mods */)
{
	int is_pressed = (action == GLFW_PRESS);
	switch (key)
	{
	case GLFW_KEY_A:
	case GLFW_KEY_ESCAPE:
		if (is_pressed)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_F:
		if (is_pressed)
		{
			flag_filaire = !flag_filaire;
			std::cout << "Mode filaire : " << ((flag_filaire) ? "ON" : "OFF") << std::endl;
		}
		break;
	case GLFW_KEY_R:
		if (is_pressed)
			flag_animate_rot_arm = 1 - flag_animate_rot_arm;
		break;
	case GLFW_KEY_T:
		if (is_pressed)
			flag_animate_rot_scale = 1 - flag_animate_rot_scale;
		break;
	case GLFW_KEY_J:
		if (dist_zoom < 60.0f)
			dist_zoom *= 1.1;
		std::cout << "Zoom is " << dist_zoom << std::endl;
		break;
	case GLFW_KEY_I:
		if (dist_zoom > 1.0f)
			dist_zoom *= 0.9;
		std::cout << "Zoom is " << dist_zoom << std::endl;
		break;
	case GLFW_KEY_UP:
		if (phy > 2)
			phy -= 2;
		std::cout << "Phy : " << phy << std::endl;
		break;
	case GLFW_KEY_DOWN:
		if (phy <= 88.)
			phy += 2;
		std::cout << "Phy : " << phy << std::endl;
		break;
	case GLFW_KEY_LEFT:
		theta -= 5;
		break;
	case GLFW_KEY_RIGHT:
		theta += 5;
		break;
	default:
		std::cout << "Touche non gérée" << std::endl;
	}
}

int main(int /* argc */, char ** /* argv */)
{
	/* GLFW initialisation */
	GLFWwindow *window;
	if (!glfwInit())
		return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	glPointSize(5.0);
	glEnable(GL_DEPTH_TEST);

	float rotation = 0.0;

	/*WORK WITH IMG*/

	/*START --- IMG parameters*/
	int IMG_WIDTH;
	int IMG_HEIGHT;
	int CANAUX;

	// EXO 1
	unsigned char *TRIFORCE_IMG = NULL;
	TRIFORCE_IMG = stbi_load("../assets/triforce.jpg", &IMG_WIDTH, &IMG_HEIGHT, &CANAUX, 0);
	if (TRIFORCE_IMG == NULL)
		std::cout << "no image(s) loaded" << std::endl;
	else
		std::cout << "ok" << std::endl;

	// EXO 2
	GLuint TEXTURE;
	glGenTextures(1, &TEXTURE);
	glBindTexture(GL_TEXTURE_2D, TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, TRIFORCE_IMG);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*END --- WORK WITH IMG*/

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2, 0.0, 0.0, 0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TEXTURE);
		// START - DESSIN QUADS
		glPushMatrix();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2d(0, 0);
		glTexCoord2f(0, 1);
		glVertex2d(0, 1);
		glTexCoord2f(1, 1);
		glVertex2d(1, 1);
		glTexCoord2f(1, 0);
		glVertex2d(1, 0);
		glEnd;
		glPopMatrix();
		// END - DESSIN QUADS
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		/* Scene rendering */
		drawFrame();
		/* Initial scenery setup */

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

		/* Animate scenery */
		rotation++;
	}
	glDeleteTextures(1, &TEXTURE);
	stbi_image_free(TRIFORCE_IMG);
	glfwTerminate();
	return 0;
}
