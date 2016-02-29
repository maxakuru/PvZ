#include "mower.h"

Mower::Mower()
{
}

Mower::Mower(int row)
{
    image = QPixmap(":/misc/lawnmower");
    boundRect = QRectF(0,(row)*113.4,83.5,110);
    speed = 20;
    isMoving = false;
    deleteMower = false;
}

void Mower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}

QRectF Mower::boundingRect() const
{
    return boundRect;
}

void Mower::advance(int phase)
{
    if(!phase) return;

    QPointF p = this->pos();

    if(isMoving && p.x() < 900)
    {
        setPos(p.x()+speed, p.y());
    }

    if(p.x() >= 900)
        deleteMower = true;

}
