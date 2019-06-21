

#ifndef food_h
#define food_h
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <list>
using namespace std;

struct point {
    int x;
    int y;
    point()
    {
        x = 0;
        y = 0;
    }
    point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    bool operator == (const point &p)
    {
        if (this->x == p.x && this->y == p.y)
            return 1;
        return 0;
    }
};

class Food
{
public:
    point loca;
    bool Eaten;
    Food();
    ~Food();
    int Produce(list<point> L);
};

#endif 
