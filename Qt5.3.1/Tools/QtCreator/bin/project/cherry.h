#ifndef CHERRY_H
#define CHERRY_H

#include "potato.h"

class Cherry : public Potato
{
public:
    Cherry();
    Cherry(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Bomb, int Splash, QPixmap Image, QRectF bRect, int y, int x);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int splash;

private:
};

#endif // CHERRY_H
