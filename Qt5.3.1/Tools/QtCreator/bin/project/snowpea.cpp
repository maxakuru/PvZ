#include "snowpea.h"

Snowpea::Snowpea()
{
}

Snowpea::Snowpea(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Slow, QPixmap Image, QRectF bRect, int y)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    range = Range;
    damage = Damage;
    rate = Rate;
    slow = Slow;
    image = Image;
    seconds = 0;
    boundRect = bRect;
    row = y;
}

QRectF Snowpea::boundingRect() const
{
    return boundRect;
}

void Snowpea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
