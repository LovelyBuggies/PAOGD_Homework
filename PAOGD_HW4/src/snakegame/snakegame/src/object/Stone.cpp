#include "Stone.h"

Stone::Stone(int pX, int pZ) {
	x = pX;
	z = pZ;
}

Stone::~Stone(){

}

void Stone::draw() {
	glPushMatrix();
	glTranslatef(-x, 0, z);
	glColor3f(.4, .4, .4);
	glutSolidDodecahedron();
	glPopMatrix();
}

float Stone::getX() {
	return x; 
}

float Stone::getZ() {
	return z; 
}

void Stone::setX(float pX) {
	x = pX; 
}

void Stone::setZ(float pZ) {
	z = pZ; 
}
