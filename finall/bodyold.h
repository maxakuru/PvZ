#ifndef BODY_H
#define BODY_H

#include <QGraphicsItem>
#include <QGraphicsScene>

class Body
{
public:
    Body();
    Body(double);
    void setPos(int x1, int y1);
    void setAngle(double a);
    void move(double time);
    void setVelocity(double v);
    static double random(double x1, double x2);
    int x()const{return xx;}
    int y()const{return yy;}
    double angle() const{return direction;}
    double getDirection();
    double getVelocity();
    double returnVY();
    double returnVX();

private:
    int xx, yy;
    double velocity, direction;
};

#endif // BODY_H
