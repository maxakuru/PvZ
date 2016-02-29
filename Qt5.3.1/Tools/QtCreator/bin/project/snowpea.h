#ifndef SNOWPEA_H
#define SNOWPEA_H

#include "peashooter.h"

class Snowpea : public Peashooter
{
public:
    Snowpea();
    Snowpea(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Slow, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int slow;

private:
};

#endif // SNOWPEA_H
