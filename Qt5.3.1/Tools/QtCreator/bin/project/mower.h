#ifndef MOWER_H
#define MOWER_H

#include <QGraphicsItem>
#include <QPainter>

class Mower : public QGraphicsItem
{
public:
    Mower();
    Mower(int row);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QRectF currentRect;
    bool isMoving;
    bool deleteMower;

protected:
    void advance(int phase);

private:
    int speed;
    QPixmap image;
    QRectF boundRect;
};

#endif // MOWER_H
