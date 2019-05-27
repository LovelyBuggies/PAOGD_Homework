#ifndef FRUIT
#define FRUIT
#include <GL/glut.h>
#include <string>

class Fruit {
public:
	Fruit(int pX, int pZ, float cR = 1., float cG = 1., float cB = 0.);
	~Fruit();
	void draw();
	float getX();
	float getZ();
	void setX(float pX);
	void setZ(float pZ);
	float getAngle();
	void setColor(float cR, float cG, float cB);
private:
	float x, z, angle,r,g,b;
};


#endif