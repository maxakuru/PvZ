#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QVector>
#include <QMessageBox>
#include "body.h"
#include "plant.h"
#include "zombie.h"
#include "bullet.h"
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

struct user
{
    int level;
    unsigned int timestamp;
    QString name;
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
    enum {WIDTH=800};
    enum {HEIGHT=500};
    QGraphicsScene *scene;
    QTimer *timer;
    void printZombies();
    void printFlowers();

private slots:

    void on_userButton_currentIndexChanged(int index);

    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> levelData[levels], zombieSeq[levels];
    QList<QString> userData[5];
    user player[5];
    int currentUser = -1;
    int currentLevel = 1;
    int limit = 5;
    int currentPoints = 50;

};

#endif // MAINWINDOW_H
