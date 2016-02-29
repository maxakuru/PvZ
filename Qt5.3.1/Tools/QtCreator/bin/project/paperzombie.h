#ifndef PAPERZOMBIE_H
#define PAPERZOMBIE_H

#include "zombietwo.h"

class PaperZombie : public ZombieTwo
{
public:
    PaperZombie();
    PaperZombie(int Number, int Life, int Life2, int Attack, double Rate, double Speed, double Speed2, QPixmap Image, QPixmap FightImage, QList<int> activeRows, int (&zombiesInRow)[5]);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    double speed2;

protected:
    void advance(int phase);

private:

};

#endif // PAPERZOMBIE_H
