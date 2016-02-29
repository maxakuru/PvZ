#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "object.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QDebug>

class Zombie : public QGraphicsItem
{
public:
    Zombie();
    Zombie(int Number, int Life, int Attack, double Rate, double Speed, QPixmap Image, QPixmap FightImage, QList<int> activeRows, int (&zombiesInRow)[5]);
    ~Zombie();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    int number;
    QPixmap regImage;
    QPixmap image;
    QPixmap fightImage;
    int life;
    int life2;
    int attack;
    double rate;
    double speed;
    double speed2;
    int randomY;
    double seconds;
    QRectF currentRect;
    QRectF boundRect;
    bool isStopped;
    bool gameOver;

protected:
    void advance(int phase);

private:


};

#endif // ZOMBIE_H
