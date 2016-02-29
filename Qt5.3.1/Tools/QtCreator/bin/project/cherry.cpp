#include "cherry.h"

Cherry::Cherry()
{
}

Cherry::Cherry(int Number, int Life, double Seeding, int Range, int Damage, double Rate, int Bomb, int Splash, QPixmap Image, QRectF bRect, int y, int x)
{
    number = Number;
    life = Life;
    seeding = Seeding;
    range = Range;
    damage = Damage;
    rate = Rate;
    bomb = Bomb;
    splash = Splash;
    image = Image;
    boundRect = bRect;
    column = x;
    row = y;
}

QRectF Cherry::boundingRect() const
{
    return boundRect;
}

void Cherry::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}
