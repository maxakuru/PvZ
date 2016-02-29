#ifndef SUNPOINT_H
#define SUNPOINT_H

#include <QGraphicsItem>
#include <QPainter>

class Sunpoint : public QGraphicsItem
{
public:
    Sunpoint();
    Sunpoint(QRectF brect);
    int randomX;
    int randomY;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QRectF currentRect;
    double seconds;

private:
    double velocity;
    QPointF destination;
    QRectF boundRect;
    int startCount;

protected:
    void advance(int phase);

};

#endif // SUNPOINT_H
