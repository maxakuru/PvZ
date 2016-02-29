#ifndef REPEATER_H
#define REPEATER_H

#include "peashooter.h"

class Repeater : public Peashooter
{
public:
    Repeater();
    Repeater(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Need, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int need;

private:
};

#endif // REPEATER_H
