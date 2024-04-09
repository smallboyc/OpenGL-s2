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

struct Vertex
{
    double pos_x{0};
    double pos_y{0};
};

Vertex origins_square;

std::vector<Vertex> vectex{};

enum class Primitives
{
    Triangle,
    Polygone,
    Point,
    Line,
    Quad
};

static Primitives primitive{Primitives::Triangle};

//--------------FONCTIONS---------------//

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void drawPrimitive(Primitives prim)
{
    glPointSize(10);
    switch (prim)
    {
    case Primitives::Triangle:
        glBegin(GL_TRIANGLES);
        break;
    case Primitives::Quad:
        glBegin(GL_QUADS);
        break;
    case Primitives::Line:
        glBegin(GL_LINES);
        break;
    case Primitives::Point:
        glBegin(GL_POINTS);
        break;
    case Primitives::Polygone:
        glBegin(GL_POLYGON);
        break;
    }

    for (Vertex v : vectex)
    {
        glVertex2d(v.pos_x, v.pos_y);
    }
    glEnd();
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

void drawSquare(bool full)
{
    float square_size{1};
    float semi_square_size{square_size / 2};
    full ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    glVertex2d(semi_square_size, semi_square_size);
    glVertex2d(semi_square_size, -semi_square_size);
    glVertex2d(-semi_square_size, -semi_square_size);
    glVertex2d(-semi_square_size, semi_square_size);
    glEnd();
}

void drawCircle(bool full)
{
    float radius{0.5f};
    int step{32};
    full ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.5f, 0.0f);
    for (int i{0}; i < 2 * step; i++)
    {
        float teta = i * M_PI / step;
        glVertex2d(radius * cos(teta), radius * sin(teta));
    }
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

// MOUSE CLICK
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &xpos, &ypos);
        if (aspectRatio >= 1)
        {
            xpos = (GL_VIEW_SIZE / 2) * (aspectRatio * (xpos * (double(2) / WINDOW_WIDTH) - 1));
            ypos = (GL_VIEW_SIZE / 2) * (-ypos * (double(2) / WINDOW_HEIGHT) + 1);
        }
        else
        {
            xpos = (GL_VIEW_SIZE / 2) * (xpos * (double(2) / WINDOW_WIDTH) - 1);
            ypos = (GL_VIEW_SIZE / 2) * (1 / aspectRatio * (-ypos * (double(2) / WINDOW_HEIGHT) + 1));
        }

        Vertex v;
        v.pos_x = xpos;
        v.pos_y = ypos;
        origins_square.pos_x = xpos;
        origins_square.pos_y = ypos;
        vectex.push_back(v);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        primitive = Primitives::Point;

    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        primitive = Primitives::Line;

    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        primitive = Primitives::Triangle;

    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        primitive = Primitives::Polygone;
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

    // KEY Pressed
    glfwSetKeyCallback(window, key_callback);
    // Mouse Pressed
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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
        // Circle
        glPushMatrix();
        glTranslatef(1.0f, 2.0f, 0.0f);
        drawCircle(true);
        glPopMatrix();

        // RED Square
        glPushMatrix();
        glRotatef(45, 0., 0., 1.);
        glTranslatef(1.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawSquare(false);
        glPopMatrix();

        // PURPLE Square
        glPushMatrix();
        glTranslatef(1.0f, 0.0f, 0.0f);
        glRotatef(45, 0., 0., 1.);
        glColor3f(1.0f, 0.0f, 1.0f);
        drawSquare(false);
        glPopMatrix();

        // GREEN Square
        glPushMatrix();
        glColor3f(0.0f, 1.0f, 0.0f);
        glTranslatef(origins_square.pos_x, origins_square.pos_y, 0.0f);
        drawSquare(false);
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
        // drawPrimitive(primitive);

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