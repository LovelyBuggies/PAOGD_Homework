

#include "food.h"
int NotInBody(list<point> L, point food_loca);
Food::Food()
{
    loca.x = -20;
    loca.y = 20;
    Eaten = false;
}
Food::~Food()
{
    
}
int Food::Produce(list<point> L)
{
    if (Eaten)
    {
        srand((unsigned)time(0));
        int low = -37;
        int high = 37;

        while (1)
        {
            loca.x = rand() % (high - low + 1) + low;
            loca.y = rand() % (high - low + 1) + low;
            if (NotInBody(L, loca))
            {
                break;
            }
        }
        return 1;
    }
    else
        return 0;
}

int NotInBody(list<point> L, point food_loca)
{
    for (auto i = L.begin(); i != L.end(); i++)
    {
        if (*i == food_loca)
        {
            return 0;
        }
    }
    return 1;
}
