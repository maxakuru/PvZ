#include "bullet.h"
#include "mainwindow.h"

Bullet::Bullet()
{
}

Bullet::Bullet(int Type, QRectF bRect, int Damage)
{
    type = Type;
    boundRect = bRect;
    speed = 10;
    damage = Damage;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(type==1) //regular bullet
    {
        QPen pen(Qt::black, 1); //black outline
        painter->setPen(pen);
        QBrush b(Qt::white); //white colour
        painter->setBrush(b);
    }
    else if(type==2) //icey bullet
    {
        QPen pen(Qt::black, 1); //black outline
        painter->setPen(pen);
        QBrush b(QColor(22,240,240)); //light blue colour
        painter->setBrush(b);
    }

    painter->drawEllipse(boundingRect());
}

QRectF Bullet::boundingRect() const
{
    return boundRect;
}

void Bullet::advance(int phase)
{
    qDebug() << "boundRect: " << boundRect;
    qDebug() << "bullet pos: " << this->pos();

    if(!phase) return;  //do nothing before advancing

    QPointF p = this->pos();    // current position in relation to it's parent (ie. parent coordinate system)
    // qDebug() << p;

    setPos(p.x()+speed, p.y()); // move based on velocity
}
