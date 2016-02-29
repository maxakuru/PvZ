#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QChar>
#include <QTextEncoder>
#include <QVector>
#include <QMessageBox>
#include "paperzombie.h"
#include "bullet.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDateTime>
#include <QTimer>
#include <QPixmap>
#include <QList>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "cherry.h"
#include "repeater.h"
#include "snowpea.h"
#include "sunflower.h"
#include "mylawn.h"
#include "sunpoint.h"
#include <QtAlgorithms>
#include <time.h>
#include "mower.h"
#include "userclass.h"

namespace Ui {
class MainWindow;
}

struct user
{
    int level;
    unsigned int timestamp;
    QString name;

    user(): level(0), timestamp(), name()
    {

    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLevel();
    int loadLevels();
    void loadUsers();
    enum{levels=8};
    void fileExistsDialog();
    int checkAlpha(QString string);
    int checkAlphaNum(QString string);
    QGraphicsScene *scene;
    QTimer *timer;
    QTimer *sunTimer;
    void printZombies();
    void printFlowers();
    qreal columnWidth;
    qreal rowHeight;
    void paintLawn();
    void rePaintLawn();
    void updateLawnXY();
    void drawPlant();
    QPointF updateClickPoint();
    void addSunpoints();
    void addSunflowerPoints(QRectF brect);
    void updateZombies();
    void updateButtons();
    void checkSunPointClick(QPointF point);
    bool cont();
    bool othercont();
    double secondsCounter;
    void updateSunflowers();
    void checkSeeding();
    bool checkPlantExists(int number);
    void checkSunpointAge();
    void setZombieTimer();
    void spawnZombie();
    void clearVectors();
    void checkShooters();
    void checkBulletCollisions();
    void checkCRPlants();
    void checkZombieAttack();
    void updatePlants();
    void deletePlantPt(QRectF rect);
    void deletePeaPt(QRectF rect);
    void removePeaShooter(QPointF point);
    void checkMowers();
    void checkMowerCollisions();
    void reAddMowers();
    void checkGameOver();

private slots:

    void on_userButton_currentIndexChanged(int index);

    void on_deleteButton_clicked();

    void on_p1_Button_clicked();
    void on_p2_Button_clicked();
    void on_p3_Button_clicked();
    void on_p4_Button_clicked();
    void on_p5_Button_clicked();
    void on_p6_Button_clicked();
    void on_p7_Button_clicked();
    void on_p8_Button_clicked();

    void on_startButton_clicked();

    void on_restartButton_clicked();

    void on_quitButton_clicked();

public slots:
    void update();

    void on_newButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> levelData[levels], zombieSeq[levels];
    MyLawn *lawn;
    QList<QString> userData[5];
    user player[5];
    QVector<UserClass*> players;
    int currentUser;
    int currentLevel;
    int limit;
    int currentPoints;
    QGraphicsRectItem cell[10][5];
    int isPainted;
    QList<QPointF> plantPt;
    QList<QPointF> peaPt;
    QVector<Plant*> plants;
    QVector<Sunflower*> sunflowers;
    QGraphicsPixmapItem *mower[5];
    int activePlant;
    QList<int> activeRows;
    QPointF clickPoint;
    QPointF lastClickPoint;
    QVector<Sunpoint*> sunpts;
    int loopcount;
    QVector<QRectF> sunptRects;
    QRectF rect[10][5];
    QRectF recto;
    double seedingTime[8];
    bool active[8];
    double zombieTimer;
    int zombieCount;
    bool zombiesKilled;
    QVector<Zombie*> zombieVector;
    QVector<Plant*> shooterPlants;
    QVector<Peashooter*> crPlants;
    QVector<Bullet*> bullets;
    QVector<Mower*> mowers;
    int zombiesInRow[5];

};

#endif // MAINWINDOW_H
