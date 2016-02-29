#include "sunpoint.h"
#include "mainwindow.h"

Sunpoint::Sunpoint()
{
    randomX = 83.6*(1+rand()%9);
    randomY = 113.2*(1+rand()%4);
    velocity = 4;
    boundRect = QRectF(randomX,0,83,113);
    currentRect = boundingRect();
    seconds = 7.5;
}

Sunpoint::Sunpoint(QRectF brect)
{
    boundRect = brect;
    velocity = 0;
    currentRect = brect;
    seconds = 7.5;
}

void Sunpoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QString filePath = ":/misc/sunpoint";
    QPixmap pixmap;
    pixmap.load(filePath);
    QRectF source(0,0,pixmap.width(),pixmap.height());
    painter->drawPixmap(boundingRect(), pixmap, source);
}

QRectF Sunpoint::boundingRect() const
{
    return boundRect;
}

void Sunpoint::advance(int phase)
{
    if(!phase) return;

    QPointF p = this->pos();    // current position in relation to it's parent (ie. parent coordinate system)
    // qDebug() << p;

    // Check boundaries
    if (p.y() >= randomY) // reverse y-direction
    {
        velocity = 0;
    }

    setPos(p.x(), p.y()+velocity); // move based on velocity

    if(velocity>0)
    {
        currentRect = QRectF(randomX,p.y(),70,70);
        seconds = 7.5;
    }
}
