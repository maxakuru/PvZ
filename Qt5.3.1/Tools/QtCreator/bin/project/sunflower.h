#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "wallnut.h"

class Sunflower : public WallNut
{
public:
    Sunflower();
    Sunflower(int Number, int Life, double Seeding, int Rate, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int rate;
    int sun;
    double seconds;

private:
};

#endif // SUNFLOWER_H
