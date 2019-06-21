

#ifndef snake_h
#define snake_h

#include <stdio.h>

#include "food.h"

enum Snake_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


class Snake
{
public:
    list<point> body;
    Snake_Movement Snake_dir;
    Snake();
    ~Snake();
    void Move();
    void Turn(Snake_Movement direction);
    int eat_food(point location);
    int collision();
};



#endif 
