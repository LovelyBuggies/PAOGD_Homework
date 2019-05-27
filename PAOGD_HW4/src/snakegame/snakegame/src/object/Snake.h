#ifndef SNAKE
#define SNAKE
#include <vector>
#include <chrono>
#include <thread>
#include "Block.h"
#include "Fruit.h"
#include "Stone.h"

enum Direction {
	LEFT, RIGHT, UP, DOWN
};

class Snake { 
public:
	std::vector<Block*> blocks; 

	Snake(float startX, float startZ, int blockCount);
	~Snake();
	void draw();

	void move(Direction direction);

	void pushSnake();

	void update(Fruit* fruit, Stone* stones[]);

	void collisionDetection(Fruit* fruit, Stone* stones[]);

	void addBlock();

	Direction getDirection();

private:
	Direction currentDirection;
	bool isUpdated;
	int score;
};

#endif