
#include "snake.h"

Snake::Snake()
{
    body.push_back(point(0, 0));
    body.push_back(point(0, -1));
    body.push_back(point(0, -2));
    body.push_back(point(0, -3));
    body.push_back(point(0, -4));
    body.push_back(point(0, -5));
    for (int i = 0; i < 20; i++)
    {
        body.push_back(point(0, -5 - i));
    }
    Snake_dir = UP;
}

Snake::~Snake()
{
    body.clear();
}

void Snake::Turn(Snake_Movement direction)
{
    if (direction == UP)
    {
        if (Snake_dir != UP && Snake_dir != DOWN)
        {
            body.pop_back();
            point temp = body.front();
            body.push_front(point(temp.x, temp.y + 1));
            Snake_dir = UP;
        }
    }
    if (direction == DOWN)
    {
        if (Snake_dir != UP && Snake_dir != DOWN)
        {
            body.pop_back();
            point temp = body.front();
            body.push_front(point(temp.x, temp.y - 1));
            Snake_dir = DOWN;
        }
    }
    if (direction == RIGHT)
    {
        if (Snake_dir != RIGHT && Snake_dir != LEFT)
        {
            body.pop_back();
            point temp = body.front();
            body.push_front(point(temp.x + 1, temp.y));
            Snake_dir = RIGHT;
        }
    }
    if (direction == LEFT)
    {
        if (Snake_dir != RIGHT && Snake_dir != LEFT)
        {
            body.pop_back();
            point temp = body.front();
            body.push_front(point(temp.x - 1, temp.y));
            Snake_dir = LEFT;
        }
    }
}

void Snake::Move()
{
    if (Snake_dir == UP)
    {
        body.pop_back();
        point temp = body.front();
        if(temp.y == 38)
            body.push_front(point(temp.x, -39));
        else
            body.push_front(point(temp.x, temp.y + 1));
    }
    if (Snake_dir == DOWN)
    {
        body.pop_back();
        point temp = body.front();
        if(temp.y == -39)
            body.push_front(point(temp.x, 38));
        else
            body.push_front(point(temp.x, temp.y - 1));
    }
    if (Snake_dir == RIGHT)
    {
        body.pop_back();
        point temp = body.front();
        if(temp.x == 38)
            body.push_front(point(-39, temp.y));
        else
            body.push_front(point(temp.x + 1, temp.y));
    }
    if (Snake_dir == LEFT)
    {
        body.pop_back();
        point temp = body.front();
        if(temp.x == -39)
            body.push_front(point(38, temp.y));
        else
            body.push_front(point(temp.x - 1, temp.y));
    }
}

int Snake::eat_food(point location)
{
    point temp = body.back();
    if (body.front() == location)
    {
        if (Snake_dir == UP)
        {
            body.push_back(point(temp.x, temp.y - 1));
        }
        else if (Snake_dir == DOWN)
        {
            body.push_back(point(temp.x, temp.y + 1));
        }
        else if (Snake_dir == RIGHT)
        {
            body.push_back(point(temp.x - 1, temp.y));
        }
        else
        {
            body.push_back(point(temp.x + 1, temp.y));
        }
        return 1;
    }
    else
        return 0;
}

int Snake::collision()
{
    int r = 0;
    list<point>::iterator i = body.begin();
    i++;
    for (; i != body.end(); i++)
    {
        if (*i == body.front())
        {
            r = 1;
        }
    }
    return r;
}
