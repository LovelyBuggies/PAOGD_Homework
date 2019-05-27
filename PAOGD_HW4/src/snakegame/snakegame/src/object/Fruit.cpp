#include "Fruit.h"

Fruit::Fruit(int pX, int pZ, float cR, float cG, float cB) {
	x = pX;
	z = pZ;
	r = cR;
	g = cG;
	b = cB;
	angle = 0.0f;
}

Fruit::~Fruit(){

}

void Fruit::draw() {
	angle += 2.0f;
	glPushMatrix();
	glTranslatef(-x, 0, z);
	glRotatef(angle, 0, 1, 0);
	glColor3f(r,g,b);
	glutSolidTeapot(.4);
}

float Fruit::getX() {
	return x; 
}

float Fruit::getZ() { 
	return z; 
}

void Fruit::setX(float pX) {
	x = pX; 
}

void Fruit::setZ(float pZ) {
	z = pZ; 
}

float Fruit::getAngle() {
	return angle; 
}

void Fruit::setColor(float cR, float cG, float cB) {
	r = cR;
	g = cG;
	b = cB;
	return;
}