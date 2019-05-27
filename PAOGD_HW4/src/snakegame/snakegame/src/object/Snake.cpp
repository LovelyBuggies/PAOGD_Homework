#include "Snake.h"

Snake::Snake(float startX, float startZ, int blockCount) {
	for (float i = 0; i < blockCount; i++) {
		Block* temp = new Block(startX + i, 0, startZ);
		if (i == 0)
			temp->setColor(1, 1, 0);
		blocks.push_back(temp);
	}
	isUpdated = false;
	currentDirection = LEFT;
	score = 0;
}

Snake::~Snake(){

}

void Snake::draw() { 
	for (int i = 0; i <= blocks.size() - 1; i++) {
		Block* temp = blocks.at(i);
		temp->draw();
	}
}


void Snake::move(Direction direction) { 
	Block* snakeHead = blocks.at(0);
	float speed = 0.0;
	if (score <= 100) { 
		speed = .3f; 
	}else if (score > 100 && score <= 200) { 
		speed = .4f; 
	}else if (score > 200 && score <= 300) { 
		speed = .5f; 
	}
	else {
		speed = .5f;
	}
	
	if (direction == LEFT && currentDirection != RIGHT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() - speed);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == RIGHT && currentDirection != LEFT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() + speed);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == UP && currentDirection != DOWN) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() + speed);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == DOWN && currentDirection != UP) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() - speed);
		currentDirection = direction;
		isUpdated = true;
	}
};

void Snake::pushSnake() { 
	for (int i = blocks.size() - 1; i >= 1; i--) {
		Block* temp = blocks.at(i);
		Block* prevBlock = blocks.at(i - 1);
		temp->setX(prevBlock->getX());
		temp->setZ(prevBlock->getZ());
	}
}

void Snake::update(Fruit* fruit, Stone* stones[]) {
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
	if (!isUpdated) 
		move(currentDirection);
	isUpdated = false;
	collisionDetection(fruit, stones);
}

void Snake::collisionDetection(Fruit* fruit, Stone* stones[]) {
	float x = blocks.at(0)->getX();
	float z = blocks.at(0)->getZ();
	if (fruit->getX() >= x-.5 && fruit->getX() <= x + .5 && fruit->getZ() >= z-.5 && fruit->getZ() <= z + .5) {
		if (score <= 100){ score += 10; }
		else if (score > 100 && score <= 200) { score += 20; }
		else if (score > 200 && score <= 300) { score += 30; }
		else score += 50;
		printf("Current Score:  %i\n", score);
		if (score == 100)
			printf("Congratulations! You pass the first level!\n");
		else if (score == 240)
			printf("Congratulations! You pass the second level!\n");
		else if (score == 480)
			printf("Congratulations! You pass the third level!\n");
		else;
		bool repeat = false;
		do { 
			repeat = false;
			fruit->setX(rand() % 11 - 5);
			fruit->setZ(rand() % 11 - 5);
			if (score % 40 == 0) {
				fruit->setColor(1.0, 1.0, 0);
			}
			else if(score %40 == 10){
				fruit->setColor(.6, 0, 0);
			}
			else if (score % 40 == 20) {
				fruit->setColor(.6, 0, .6);
			}
			else {
				fruit->setColor(0, 0.4, 0);
			}
			for (int i = 0; i <= blocks.size() - 1;i++) {
				x = blocks.at(i)->getX();
				z = blocks.at(i)->getZ();
				if (fruit->getX() == x && fruit->getZ() == z)
					repeat = true;
			}

		} while (repeat);
		for (int i = 0; i <= score / 100;i++)
			addBlock();
	}

	Block* snakeHead = blocks.at(0);
	if (-6 >= x && currentDirection == LEFT) {
		pushSnake();
		snakeHead->setX(6.0f);
		currentDirection = LEFT;
		isUpdated = true;
	}
	else if (x >= 6 && currentDirection == RIGHT) {
		pushSnake();
		snakeHead->setX(-6.0f);
		currentDirection = RIGHT;
		isUpdated = true;
	}
	else if (z >= 6 && currentDirection == UP) {
		pushSnake();
		snakeHead->setZ(-6.0f);
		currentDirection = UP;
		isUpdated = true;
	}
	else if (z <= -6 && currentDirection == DOWN) {
		pushSnake();
		snakeHead->setZ(6.0f);
		currentDirection = DOWN;
		isUpdated = true;
	}
	for (int i = 0; i < 1; i++) {
		if (stones[i]->getX() >= x - .3 && stones[i]->getX() <= x + .3 && stones[i]->getZ() >= z - .3 && stones[i]->getZ() <= z + .3) {
			printf("Smash to pieces... \n");
			glutHideWindow();
			break;
		}
	}
	for (int i = 1; i <= blocks.size() - 1; i++) { 
		if (blocks.at(i)->getX() == blocks.at(0)->getX() && blocks.at(i)->getZ() == blocks.at(0)->getZ()) {
			printf("Eat yourself... \n"); 
			glutHideWindow();
			break;
		}
	}
};

void Snake::addBlock() { 
	Block* newBlock = new Block(-100, 0, -100);
	blocks.push_back(newBlock);
};

Direction Snake::getDirection() {
	return currentDirection; 
}
