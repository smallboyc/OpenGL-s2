#include "draw_scene.hpp"
#include "3D_tools.hpp"

float height{10.0};

void drawFrame()
{
    float size{16.0};
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.01);
    glPushMatrix();
    // X
    glBegin(GL_LINES);
    glColor3f(size, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(size, 0.0, 0.0);
    glEnd();

    // Y
    glBegin(GL_LINES);
    glColor3f(0.0, size, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, size, 0.0);
    glEnd();

    // Z
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, size);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, size);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawBase()
{
    // Circle
    glColor3f(235.0 / 255.0, 207.0 / 255.0, 52.0 / 255.0);
    glPushMatrix();
    glScalef(3, 3, 1);
    drawCircle();
    glPopMatrix();
    // Cone
    glPushMatrix();
    glScalef(2, 2, height);
    drawCone();
    glPopMatrix();
}

void drawArm()
{
    glPushMatrix();
    glColor3f(245.0 / 255.0, 164.0 / 255.0, 66.0 / 255.0);
    // glTranslatef(0, 0, height);
    // SPHERE
    glPushMatrix();
    glScalef(1.6, 1.6, 1.6);
    drawSphere();
    glPopMatrix();

    // CONES
    //  Cone 1
    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    glScalef(1, 1, height);
    drawCone();
    glPopMatrix();
    // Cone 2
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glScalef(1, 1, height);
    drawCone();
    glPopMatrix();
    glPopMatrix();
}

void drawLines()
{
    float radius{3.f};
    int step{3};
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i{1}; i < 2 * step; i++)
    {
        float teta = 2 * i * M_PI / step;
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(radius * cos(teta), radius * sin(teta), -5.0);
    }
    glEnd();
}

void drawPan(int angle)
{
    glPushMatrix();

    // PAN 1
    glPushMatrix();
    glTranslatef(0, -10, 0);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    // Circle 1
    glPushMatrix();
    glColor3f(89.0 / 255.0, 115.0 / 255.0, 0.0);
    glTranslatef(0, 0, -height / 2);
    glScalef(3, 3, 1);
    drawCircle();
    glPopMatrix();

    // Line 1
    glPushMatrix();
    drawLines();
    glPopMatrix();
    glPopMatrix();

    // PAN 2
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    // Circle 2
    glPushMatrix();
    glColor3f(89.0 / 255.0, 115.0 / 255.0, 0.0);
    glTranslatef(0, 0, -height / 2);
    glScalef(3, 3, 1);
    drawCircle();
    glPopMatrix();

    // Line 2
    glPushMatrix();
    drawLines();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}
