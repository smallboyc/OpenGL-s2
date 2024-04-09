#define GLFW_INCLUDE_NONE
#define _USE_MATH_DEFINES

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <OpenGL/gl.h>
#include <iostream>
#include <cmath>

#include "3D_tools.hpp"
#include "draw_scene.hpp"

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "TD04 Ex01";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* IHM flag */
static int flag_filaire = 0;
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

float sign = 1;

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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (flag_filaire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		/* Initial scenery setup */
		glPushMatrix();
		glTranslatef(0.0, 0.0, -0.01);
		glScalef(10.0, 10.0, 1.0);
		glColor3f(0.0, 0.0, 0.1);
		drawSquare();
		glBegin(GL_POINTS);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();
		glPopMatrix();

		/* Scene rendering */
		drawFrame();

		// SPHERE ROTATION--start
		// float radius{4};
		// glColor3f(1, 0, 0);
		// glPushMatrix();
		// glRotatef(flag_animate_rot_arm, 0.0f, 0.0f, 1.0f); // Rotation autour de l'axe Z
		// glTranslatef(radius, 0, 5);
		// drawSphere();
		// glPopMatrix();
		// /* Increment flag_animate_rot_arm for rotation */
		// flag_animate_rot_arm += 1; // Adjust rotation speed as needed
		// SPHERE ROTATION--end

		glColor3f(1, 0, 0);
		glPushMatrix();
		glRotatef(flag_animate_rot_scale, 0.0f, 0.0f, 1.0f); // Rotation autour de l'axe Z
		glPushMatrix();
		glTranslatef(0, 0, 10);
		glRotatef(flag_animate_rot_arm, 1.0f, 0.0f, 0.0f);
		drawArm();
		drawPan(-flag_animate_rot_arm); // IMPORTANT!!!!, cibler l'animation selon l'origine de l'élément (rotation -> translation -> scale)
		glPopMatrix();
		glPopMatrix();
		/* Increment flag_animate_rot_arm for rotation */
		if (flag_animate_rot_arm == 30)
			sign *= -1; // Adjust rotation speed as needed
		else if (flag_animate_rot_arm == -30)
			sign *= -1;
		flag_animate_rot_arm += sign;
		flag_animate_rot_scale += 1;

		// CONE STATIQUE
		drawBase();
		// drawArm();
		// drawPan();

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
	}

	glfwTerminate();
	return 0;
}
