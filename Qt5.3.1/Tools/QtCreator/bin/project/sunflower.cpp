#include "sunflower.h"

Sunflower::Sunflower()
{
}

Sunflower::Sunflower(int Number, int Life, double Seeding, int Rate, QPixmap Image, QRectF bRect, int y)
{
    boundRect = bRect;
    life = Life;
    number = Number;
    seeding = Seeding;
    rate = Rate;
    image = Image;
    seconds = 0;
    row = y;
}

QRectF Sunflower::boundingRect() const
{
    return boundRect;
}

void Sunflower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
