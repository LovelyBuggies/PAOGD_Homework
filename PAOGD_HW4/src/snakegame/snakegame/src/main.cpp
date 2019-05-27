#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "object/Fruit.h"
#include "object/Block.h"
#include "object/Snake.h"
#include "object/Stone.h"

#include "draw/Draw.h"
#include "sys.h"

float yCamPos = 13.0f;
GLfloat lightAmbient[] = { 0.5,0.5,0.5,1 };
GLfloat lightDiffuse[] = { 1,1,1,1 };
GLfloat lightPosition[] = { 0,3,0 };

Fruit* fruit = new Fruit(1, 1);

Snake* snake = new Snake(0, 0, 3);

Stone* stone = new Stone(-3,-3);
Stone* stones[2] = { stone };
void resize(int w, int h) {
	if (h == 0)
		h = 1;
	float ratio = w * 1.0f / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(50, ratio, 1, 200);
	glMatrixMode(GL_MODELVIEW);
}

void keyEvents(unsigned char key, int x, int y) { 
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 'a':
		snake->move(LEFT);
		break;
	case 'd':
		snake->move(RIGHT);
		break;
	case 'w':
		snake->move(UP);
		break;
	case 's':
		snake->move(DOWN);
		break;
	case 'Q': 
		exit(0);
		break;
	case 'A':
		snake->move(LEFT);
		break;
	case 'D':
		snake->move(RIGHT);
		break;
	case 'W':
		snake->move(UP);
		break;
	case 'S':
		snake->move(DOWN);
		break;
	}
}

void specialKeys(int key, int x, int y) { 
	switch (key) {
	case GLUT_KEY_F1:
		exit(0);
		break;
	}
}

void mouseEvents(int button, int dir, int x, int y) {
	switch (button) {
	case 3:
		yCamPos -= 0.5;
		break;
	case 4:
		yCamPos += 0.5;
		break;
	}
}

void draw() {
	snake->update(fruit, stones);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		0, yCamPos, -5,
		0, 0, 0,
		0, 1, 0
		);
	Draw d;
	d.drawGrid();
	snake->draw();
	// build stones
	stone->draw();
	fruit->draw();


	glutSwapBuffers();
}

void initGL(void) {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Greedy Snake");

	initGL();
	Draw d;
	d.buildDLs();

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyEvents);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseEvents);

	glutMainLoop();

	return 1;
}
