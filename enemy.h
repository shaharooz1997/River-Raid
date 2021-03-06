#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsItem>
#include <QTimer>
#include <QGraphicsPixmapItem>


class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *parent=0);
    ~Enemy();

public slots:
    void move();
    void movex();

protected:
    int xspeed;
    bool direction;
    bool movingx;
    QTimer *timer;
    QTimer *timer2;
    QString str1;//right pic
    QString str2;//left pic
};

#endif // ENEMY_H
