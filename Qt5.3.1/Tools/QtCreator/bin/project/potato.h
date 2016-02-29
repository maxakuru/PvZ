#ifndef POTATO_H
#define POTATO_H

#include "peashooter.h"

class Potato : public Peashooter
{
public:
    Potato();
    Potato(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Bomb, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int bomb;

private:
};

#endif // POTATO_H
