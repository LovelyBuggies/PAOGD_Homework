#include "Block.h"

float iniR = 255; float iniG = 0; float iniB = 0; 

Block::Block(float pX, float pY, float pZ, float pSize) {
	x = pX;
	y = pY;
	z = pZ;
	size = pSize;
	r = iniR; g = iniG; b = iniB; 
}

Block::~Block(){

}

void Block::setX(float pX) { 
	x = pX; 
}

void Block::setY(float pY) { 
	y = pY; 
}

void Block::setZ(float pZ) { 
	z = pZ; 
}

float Block::getX() { 
	return x; 
}

float Block::getY() {
	return y; 
}

float Block::getZ() { 
	return z; 
}

void Block::setColor(float cR, float cG, float cB) {
	r = cR;g = cG;b = cB; 
}

//function to draw the block
void Block::draw() { 
	glPushMatrix();
	glTranslatef(-x, y, z);
	if (r > 1 || g > 1 || b > 1) {
		glColor3ub(r, g, b);
	}
	else {
		glColor3f(r, g, b);
	}

	// glCallList(cube);
	glutSolidSphere(0.4,10,10);
	glPopMatrix();
}