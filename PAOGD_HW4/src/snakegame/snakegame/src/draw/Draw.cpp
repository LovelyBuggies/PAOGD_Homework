#include "Draw.h"

Draw::Draw(){

}

Draw::~Draw(){

}

void Draw::buildDLs() { 
	cube = glGenLists(1);
	glNewList(cube, GL_COMPILE);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0.5, 0.5, 0.5); 
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(0, 0, -1);
	glVertex3f(0.5, 0.5, -0.5); 
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(0, -1, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(0, 1, 0); 
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(1, 0, 0); 
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
	glEndList();
}



void Draw::drawGrid() { 
	glColor3ub(155, 155, 155);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (float i = -7; i <= 5; i+=.5) {
		glVertex3f(-5 - 0.5, 0 - 0.5f, i + 0.5f);
		glVertex3f(6 - 0.5, 0 - 0.5f, i + 0.5f);
	}
	for (float i = -5; i <= 6; i+=.5) {
		glVertex3f(i - 0.5, -0.5f, -6 - 0.5f);
		glVertex3f(i - 0.5, -0.5f, 6 - 0.5f);
	}
	glEnd();
	glLineWidth(1);
	glColor3ub(20, 20, 20);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5 - 0.5, -0.51, -6.5);
	glVertex3f(-5 - 0.5, -0.51, 6.5);
	glVertex3f(6 - 0.5, -0.51, 6.5);
	glVertex3f(6 - 0.5, -0.51, -6.5);
	glEnd();

	glColor3ub(40, 40,40);
	for (int i = -5; i <= 5; i++) {
		glPushMatrix();
		glTranslatef(-6, 0, i);
		glCallList(cube);

		float cR = 1; float cG = 1; float cB = 1; 
		glColor3f(cR, cG, cB);
		glLineWidth(0);
		glBegin(GL_LINES);
		glVertex3f(0, 0.51, -0.5);
		glVertex3f(0, 0.51, 0.5);
		glEnd();
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(-0.5, 0.51, 0.5);
		glEnd();
		glColor3ub(40,40, 40);


		glPopMatrix();
		glPushMatrix();
		glTranslatef(6, 0, i);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(0);
		glBegin(GL_LINES);
		glVertex3f(0, 0.51, -0.5);
		glVertex3f(0, 0.51, 0.5);
		glEnd();
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(-0.5, 0.51, 0.5);
		glEnd();
		glColor3ub(40, 40, 40);

		glPopMatrix();
		glPushMatrix();
		glTranslatef(i, 0, -6);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(0);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0);
		glVertex3f(0.5, 0.51, 0);
		glEnd();
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, -0.5);
		glEnd();
		glColor3ub(40, 40, 40);

		glPopMatrix();
		glPushMatrix();
		glTranslatef(i, 0, 6);
		glCallList(cube);

		glColor3f(cR, cG, cB);
		glLineWidth(0);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0);
		glVertex3f(0.5, 0.51, 0);
		glEnd();
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(-0.5, 0.51, 0.5);
		glVertex3f(0.5, 0.51, 0.5);
		glVertex3f(-0.5, 0.51, -0.5);
		glVertex3f(0.5, 0.51, -0.5);
		glEnd();
		glColor3ub(40, 40, 40);

		glPopMatrix();
	}
}