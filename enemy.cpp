#include "enemy.h"
#include "game.h"
#include <QList>
#include <typeinfo>

extern Game *game;
extern int yspeed;

Enemy::Enemy(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    /* a random number */
    int xPos = qrand()%50;
    int yPos = -80;


    int f = qrand()%2;
    if(f)
        xPos += 200;
    else
        xPos += 500;
    setPos(xPos,yPos);
    direction = qrand()%2;//1: right     0:left

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(40);

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(movex()));
    movingx=0;

}


Enemy::~Enemy()
{
    delete timer;
    delete timer2;
}

void Enemy::move()
{
    setPos(x(),y()+yspeed);
    if(movingx == 0 && pos().y()+pixmap().height()>250)
    {
        timer2->start(50);
        movingx=1;
    }
    if(pos().y()+pixmap().height()>600) {
        game->scene()->removeItem(this);
        delete this;
    }
    return;
}

void Enemy::movex()
{
    setPos(x()+xspeed, y());

    QList<QGraphicsItem *> colliding_items = collidingItems();

    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if(typeid(*(colliding_items[i])) == typeid(Wall) && typeid(*this) != typeid(Jet))
        {
            xspeed *= -1;
            if(direction == 1)
            {
                setPixmap(QPixmap(str2));
                direction = 0;
            }
            else
            {
                setPixmap(QPixmap(str1));
                direction = 1;
            }
        }
    }
    return;
}
