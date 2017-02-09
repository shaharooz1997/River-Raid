#include "game.h"
#include <QDebug>
#include "enemy.h"
#include "jet.h"
#include "map_start.h"
#include <QtCore>
#include <QtGui>
#include <typeinfo>

Game::Game(QWidget *parent)
{
    // create the scene
    _scene = new QGraphicsScene();
    _scene->setSceneRect(0,0,800,600);

    //initialize the view window
    this->setScene(_scene);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFixedSize(800,600);
    this->setBackgroundBrush(QBrush(QImage(":/images/bg.png")));

    //initialize the airplane

//    this->create_map();

    //the more initialize must be here...

//    qDebug() <<"game constructor ran.";

}

QGraphicsScene *Game::scene()
{
    return this->_scene;
}

Airplane *Game::airplane()
{
    return this->_airplane;
}

Footer *Game::foot()
{
    return _foot;
}

Score *Game::score()
{
    return _score;
}

Health *Game::health()
{
    return _health;
}

Game::~Game()
{
    delete _airplane;
    delete _scene;
//    delete bgsound;
    delete _foot;
    delete timer;
}

void Game::game_over()
{
    timer->stop();
    _airplane->getTimer()->stop();
    _airplane->getTimer2()->stop();
    QList<QGraphicsItem*> mitem = _scene->items();
    //health,score,foot,aiplane
    foreach (QGraphicsItem* it, mitem) {
        it->setEnabled(false);
    }

    QGraphicsRectItem* panel1 = new QGraphicsRectItem(0,0,800,600);
    panel1->setBrush(QBrush(QImage(":/images/menu.png")));
    panel1->setOpacity(0.15);
    this->scene()->addItem(panel1);

    QGraphicsRectItem* panel2 = new QGraphicsRectItem(300,200,200,200);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkCyan);
    panel2->setBrush(brush);
    panel2->setOpacity(0.75);
    this->scene()->addItem(panel2);

    QGraphicsTextItem *gameOvr =new QGraphicsTextItem(QString("GameOver"));
    QFont titleFont("comic sans", 15);
    gameOvr->setFont(titleFont);
    int tXpos = rect().width()/2 - gameOvr->boundingRect().width()/2;
    int tYpos = 195;
    gameOvr->setPos(tXpos,tYpos);
    scene()->addItem(gameOvr);
    QPushButton *play = new QPushButton(QString("Play"));
    play->setGeometry(rect().width()/2 - 50,225,100,60);
    scene()->addWidget(play);
    connect(play,SIGNAL(clicked()),this,SLOT(start()));
    QPushButton *cancel = new QPushButton(QString("Quit"));
    cancel->setGeometry(rect().width()/2 - 50,315,100,60);
    scene()->addWidget(cancel);
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));

    return;
}

void Game::create_map()
{
//    Map_start *a = new Map_start(1);
    new Road();
}

void Game::dispalyMenu() {
    QGraphicsTextItem *titleText =new QGraphicsTextItem(QString("RiverRaid"));
    this->setBackgroundBrush(QBrush(QImage(":/images/menu.png")));
    QFont titleFont("comic sans", 50);    
    titleText->setFont(titleFont);
    int tXpos = rect().width()/2 - titleText->boundingRect().width()/2;
    int tYpos = 150;
    titleText->setPos(tXpos,tYpos);
    scene()->addItem(titleText);
    QPushButton *play = new QPushButton(QString("Play"));
    play->setGeometry(rect().width()/2 - 50,250,100,60);
    scene()->addWidget(play);
    connect(play,SIGNAL(clicked()),this,SLOT(start()));
    QPushButton *cancel = new QPushButton(QString("Quit"));
    cancel->setGeometry(rect().width()/2 - 50,350,100,60);
    scene()->addWidget(cancel);
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
}

void Game::dec_health()
{
    _health->dec_health();
    QList<QGraphicsItem*> mitem = _scene->items();
    //health,score,foot,aiplane
    foreach (QGraphicsItem* it, mitem) {
        if((typeid(*it) != typeid(Airplane)) && (typeid(*it) != typeid(Score)) && (typeid(*it) != typeid(Health)) && (typeid(*it) != typeid(Footer)) &&  (typeid(*it) != typeid(QGraphicsPixmapItem)))
        this->scene()->removeItem(it);
    }
    if(_health->get_health() == 0) {
        game_over();
        return;
    }
    restart();

//    this->scene()->addItem(this->_airplane);
}

void Game::restart()
{
        _airplane->setPos(380,550-115);
        _airplane->re_fuel();        
}

void Game::start()
{
//    _scene->clear();
   QList<QGraphicsItem*> mitem = _scene->items();
   foreach (QGraphicsItem* it, mitem) {
       _scene->removeItem(it);       
   }      
    this->setBackgroundBrush(QBrush(QImage(":/images/bg.png")));
    this->_airplane = new Airplane();
    //must change and enhance
    this->_airplane->setFlag(QGraphicsItem::ItemIsFocusable);
    this->_airplane->setFocus();
    this->_airplane->setZValue(10);
    this->scene()->addItem(this->_airplane);

//    this->bgsound = new QMediaPlayer();
//    this->bgsound->setMedia(QUrl("qrc:/sounds/bg.mp3"));
//    this->bgsound->play();

    //set foot
    _foot = new Footer();
    this->scene()->addItem(this->_foot);
    this->scene()->addItem(this->_foot->slider());
    this->_foot->setZValue(10);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(make_enemy()));
    timer->start(2000);

    _score = new Score();
    scene()->addItem(_score);

    _health = new Health();
    scene()->addItem(_health);

}


void Game::make_enemy()
{
    int a=((int)qrand())%4;
    if(a == 0)
        scene()->addItem(new Boat());
    else if(a == 1)
        scene()->addItem(new Fuel_depot());
    else if(a == 2)
        scene()->addItem(new Jet());
    else if(a == 3)
        scene()->addItem(new Helicopter());

    return;
}
