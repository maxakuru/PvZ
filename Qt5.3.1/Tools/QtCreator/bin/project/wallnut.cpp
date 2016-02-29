#include "wallnut.h"

WallNut::WallNut()
{
}

WallNut::WallNut(int Number, int Life, double Seeding, QPixmap Image, QRectF bRect, int y)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    image = Image;
    boundRect = bRect;
    row = y;
}

QRectF WallNut::boundingRect() const
{
    return boundRect;
}

void WallNut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
