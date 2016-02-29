#ifndef WALLNUT_H
#define WALLNUT_H

#include <QGraphicsItem>
#include "plant.h"

class WallNut : public Plant
{
public:
    WallNut();
    WallNut(int Number, int Life, double Seeding, QPixmap Image, QRectF bRect, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    double seeding;
    QRectF boundRect;
    QPixmap image;
    int column;

signals:

public slots:

private:

};

#endif // WALLNUT_H
