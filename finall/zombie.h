#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "body.h"

class Zombie : public QGraphicsItem
{
public:
    Zombie();
    void setBody(Body b);
    void move(double time);
    //virtual void draw() = 0;
    void plot(Zombie &a);
    double vx, vy;

protected:
    Body posVel;
    void advance(int phase);
};

#endif // ZOMBIE_H
