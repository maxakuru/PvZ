#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <qmath.h>
#include <iostream>
#include <QPixmap>

class Object
{
public:
    Object();
    Object(double);
    void setVelocity(double v);
    int x()const{return xx;}
    int y()const{return yy;}
    double getVelocity();
    double returnVY();
    double returnVX();

private:
    int xx, yy;
    double velocity;
};

#endif // OBJECT_H
