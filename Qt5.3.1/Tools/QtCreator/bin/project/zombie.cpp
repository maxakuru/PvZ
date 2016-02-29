#include "zombie.h"

Zombie::Zombie()
{
}

Zombie::Zombie(int Number, int Life, int Attack, double Rate, double Speed, QPixmap Image, QPixmap FightImage, QList<int> activeRows, int (&zombiesInRow)[5])
{
    number = Number;
    life = Life;
    attack = Attack;
    rate = Rate;
    speed = (83/(Speed*10));
    regImage = Image;
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


    qDebug() << "zombie pos: " << this->pos();
    qDebug() << boundRect;
}

Zombie::~Zombie()
{
}

void Zombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF source(0,0,image.width(),image.height());
    painter->drawPixmap(boundingRect(), image, source);
}

QRectF Zombie::boundingRect() const
{
    return boundRect;
}

void Zombie::advance(int phase)
{
    qDebug()<< "zombie pos: " << this->pos();

    if(!phase) return;

    QPointF p = this->pos();
    // qDebug() << p;

    if(!isStopped)
    {
        setPos(p.x()-speed, p.y()); // move based on speed
        image = regImage;
    }
    else
        image = fightImage;

    if(p.x() <= -840)
        gameOver = true;
}
