#include "paperzombie.h"

PaperZombie::PaperZombie()
{
}

PaperZombie::PaperZombie(int Number, int Life, int Life2, int Attack, double Rate, double Speed, double Speed2, QPixmap Image, QPixmap FightImage, QList<int> activeRows, int (&zombiesInRow)[5])
{
    number = Number;
    life = Life;
    life2 = Life2;
    attack = Attack;
    rate = Rate;
    speed = 83/(Speed*10);
    speed2 = 83/(Speed2*10);
    image = Image;
    fightImage = FightImage;
    isStopped = false;
    gameOver = false;

    srand(time(NULL));
    //generate random row for zombie to walk down
    if(activeRows.size() == 1)
        randomY = 2;
    else if(activeRows.size() == 3)
        randomY = (1+rand()%3);
    else if(activeRows.size() == 5)
        randomY = (rand()%5);

    boundRect = QRectF(871,randomY*114,87,111);

    currentRect = boundRect;

    //add zombie to zombiesinrow
    zombiesInRow[randomY]++;
}

void PaperZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}

QRectF PaperZombie::boundingRect() const
{
    return boundRect;
}

void PaperZombie::advance(int phase)
{
    //if()
    if(!phase) return;  // We don't do anything to prepare objects for advancing

    QPointF p = this->pos();    // current position in relation to it's parent (ie. parent coordinate system)
    // qDebug() << p;

    if(!isStopped)
    {
        setPos(p.x()-speed, p.y()); // move based on velocity
        image = regImage;
    }

    else
        image = fightImage;

    if(p.x() <= -840)
        gameOver = true;

    if(life2 == 0)
    {
        image = QPixmap(":/misc/regzomb");
        fightImage = QPixmap(":/misc/regzombfight");
    }

}
