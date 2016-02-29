#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "wallnut.h"

class Peashooter : public WallNut
{
public:
    Peashooter();
    Peashooter(int Number, int Life, double Seeding, int Range, int Damage, double Rate, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int damage;
    int range;

private:
};

#endif // PEASHOOTER_H
