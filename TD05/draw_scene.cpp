#include "draw_scene.hpp"
#include "3D_tools.hpp"

void drawFrame() {
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,1.0,0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();
}
