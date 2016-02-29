#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsItem>
#include <QPainter>
#include <qdebug.h>

class Bullet : public QGraphicsItem
{
public:
    Bullet();
    Bullet(int Type, QRectF bRect, int Damage);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QRectF currentRect;
    int damage;
    int type;

protected:
    void advance(int phase);

private:
    int speed;
    QRectF boundRect;
};

#endif // BULLET_H
