#ifndef ZOMBIETWO_H
#define ZOMBIETWO_H

#include "zombie.h"

class ZombieTwo : public Zombie
{
public:
    ZombieTwo();
    ZombieTwo(int Number, int Life, int Life2, int Attack, double Rate, double Speed, QPixmap Image, QPixmap FightImage, QList<int> activeRows, int (&zombiesInRow)[5]);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    int life2;

protected:
    void advance(int phase);

private:
};

#endif // ZOMBIETWO_H
