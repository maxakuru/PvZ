#ifndef PLANT_H
#define PLANT_H

#include "body.h"

class Plant : public QGraphicsItem
{
public:
    Plant();
    void setBody(Body b);
    void move(double time);
    //virtual void draw() = 0;
    void plot(Plant &a);
    double vx, vy;

protected:
    Body posVel;
    void advance(int phase);

private:
    int number;
    int cost;
    int life;
    int range;
    int damage;
    double rate;
    int splash;
    int slow;
    int bomb;
    double seedling;
    int sun;
    int need;
};

#endif // PLANT_H
