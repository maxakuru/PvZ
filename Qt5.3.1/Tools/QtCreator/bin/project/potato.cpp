#include "potato.h"

Potato::Potato()
{
}

Potato::Potato(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Bomb, QPixmap Image, QRectF bRect, int y)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    range = Range;
    damage = Damage;
    rate = Rate;
    bomb = Bomb;
    image = Image;
    boundRect = bRect;
    row = y;
}

QRectF Potato::boundingRect() const
{
    return boundRect;
}

void Potato::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
