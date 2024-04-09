#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <OpenGL/gl.h>
#include <iostream>
#include <vector>
#include <math.h>

//---------VARIABLES DEFINITIONS-----------//

/* Minimal time wanted between two images */
static const float GL_VIEW_SIZE = 6;
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;
static float aspectRatio;

//--------------FONCTIONS---------------//

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void drawOrigin()
{
    glPointSize(10);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(1, 0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(0, 1);
    glEnd();
}

float square_size{1};
float semi_square_size{square_size / 2};

void drawSquare(bool full)
{

    full ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    glVertex2d(semi_square_size, semi_square_size);
    glVertex2d(semi_square_size, -semi_square_size);
    glVertex2d(-semi_square_size, -semi_square_size);
    glVertex2d(-semi_square_size, semi_square_size);
    glEnd();
}

float l{0.05};
float L{0.46};

float radius{0.2f};
float dist_btw_circle{0.6f};
float dist_btw_rounded_square{0.46f};

void drawCircle(bool full)
{

    int step{32};
    full ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    for (int i{0}; i < 2 * step; i++)
    {
        float teta = i * M_PI / step;
        glVertex2d(radius * cos(teta), radius * sin(teta));
    }
    glEnd();
}

void drawTrapeze(bool full)
{
    full ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    glVertex2d(0, radius);
    glVertex2d(0.6, radius / 2);
    glVertex2d(0.6, -radius / 2);
    glVertex2d(0, -radius);
    glEnd();
}

void onWindowResized(GLFWwindow *window, int width, int height)
{
    aspectRatio = width / static_cast<float>(height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1)
    {
        glOrtho(
            -GL_VIEW_SIZE / 2.0f * aspectRatio, GL_VIEW_SIZE / 2.0f * aspectRatio,
            -GL_VIEW_SIZE / 2.0f, GL_VIEW_SIZE / 2.0f, -1.0, 1.0);
    }
    else
    {
        glOrtho(
            -GL_VIEW_SIZE / 2.0f, GL_VIEW_SIZE / 2.0f,
            -GL_VIEW_SIZE / 2.0f / aspectRatio, GL_VIEW_SIZE / 2.0f / aspectRatio, -1.0, 1.0);
    }
}

void drawFirstArm()
{
    glColor3f(1.0f, 0.0f, 1.0f);

    // CYLINDRE-start
    glPushMatrix();
    drawTrapeze(false);
    drawCircle(true);
    glTranslatef(dist_btw_circle, 0, 0);
    glScalef(0.5, 0.5, 1);
    drawCircle(true);
    glPopMatrix();
    // CYLINDRE-end
}

void drawCross()
{
    drawSquare(true);
    // ext droite
    glPushMatrix();
    glTranslatef(square_size, 0, 0);
    drawSquare(true);
    glPopMatrix();
    // ext gauche
    glPushMatrix();
    glTranslatef(-square_size, 0, 0);
    drawSquare(true);
    glPopMatrix();
    // ext haut
    glPushMatrix();
    glTranslatef(0, square_size, 0);
    drawSquare(true);
    glPopMatrix();
    // ext bas
    glPushMatrix();
    glTranslatef(0, -square_size, 0);
    drawSquare(true);
    glPopMatrix();
}

void drawRoundedCoins()
{
    glPushMatrix();
    glTranslated(semi_square_size, semi_square_size, 0);
    glScalef(5, 5, 1);
    drawCircle(true);
    glPopMatrix();
    glPushMatrix();
    glTranslated(semi_square_size, -semi_square_size, 0);
    glScalef(5, 5, 1);
    drawCircle(true);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-semi_square_size, -semi_square_size, 0);
    glScalef(5, 5, 1);
    drawCircle(true);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-semi_square_size, semi_square_size, 0);
    glScalef(5, 5, 1);
    drawCircle(true);
    glPopMatrix();
}

void drawRoundedSquare()
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(.04f, .04f, 1.f);
    drawCross();
    drawRoundedCoins();
    glPopMatrix();
}

void drawSecondArm()
{
    glPushMatrix();
    glTranslatef(dist_btw_circle, 0, 0);
    glPushMatrix();
    drawRoundedSquare();
    glTranslatef(dist_btw_rounded_square, 0, 0);
    drawRoundedSquare();
    glPopMatrix();
    glPopMatrix();
}

//------MAIN---------//

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

    //----START EVENTS_LISTENER ----//

    // WINDOW RESIZED
    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowSizeCallback(window, onWindowResized);

    //---END EVENTS_LISTENER---//

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double startTime = glfwGetTime();

        /* Render here */

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 0.0);

        //-------Start drawing
        drawOrigin();

        glPushMatrix();
        glScalef(2, 2, 1);
        drawFirstArm();
        drawSecondArm();
        glPopMatrix();

        //----------End drawing

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