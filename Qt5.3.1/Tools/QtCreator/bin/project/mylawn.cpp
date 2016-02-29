#include "mylawn.h"

MyLawn::MyLawn(QWidget *parent) :
    QGraphicsView(parent)
{
    clickpt = QPointF(0,0);
}

void MyLawn::mousePressEvent(QMouseEvent * event)
{
    //clickpt = event->pos();
    clickpt = event->pos();

    //mainWindow->drawPlant(event->pos());
    //qDebug() << mainWindow->scene;
}

QPointF MyLawn::getClickPoint() const
{
    return clickpt;
}
