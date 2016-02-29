#include "repeater.h"

Repeater::Repeater()
{
}

Repeater::Repeater(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Need, QPixmap Image, QRectF bRect, int y)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    range = Range;
    damage = Damage;
    rate = Rate;
    need = Need;
    image = Image;
    seconds = 0;
    boundRect = bRect;
    row = y;
}

QRectF Repeater::boundingRect() const
{
    return boundRect;
}

void Repeater::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
