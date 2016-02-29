#include "peashooter.h"

Peashooter::Peashooter()
{
}

Peashooter::Peashooter(int Number, int Life, double Seeding, int Range, int Damage, double Rate, QPixmap Image, QRectF bRect, int y)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    range = Range;
    damage = Damage;
    rate = Rate;
    image= Image;
    seconds = 0;
    boundRect = bRect;
    row = y;
}

QRectF Peashooter::boundingRect() const
{
    return boundRect;
}

void Peashooter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
