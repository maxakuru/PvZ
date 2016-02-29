#ifndef PLANT_H
#define PLANT_H

#include "object.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>

class Plant : public QGraphicsItem
{
public:
    Plant();
    double seconds;
    int life;
    int number;
    int rate;
    int row;

protected:

private:
    QRectF boundRect;
    QPixmap image;
};

#endif // PLANT_H

