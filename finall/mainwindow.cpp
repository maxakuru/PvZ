#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    int check = loadLevels();
    if(check==0)
        ui->quitButton->clicked();

    loadUsers();

    if(currentUser<0)
    {
        ui->deleteButton->setDisabled(true);
        ui->startButton->setDisabled(true);
        ui->restartButton->setDisabled(true);
    }

    scene = new QGraphicsScene(this);   // scene holds all objects in the scene
    ui->graphicsView->setScene(scene);  // graphicsView is the viewport on to the scene
    QRectF rect(0,0,WIDTH,HEIGHT);
    scene->setSceneRect(rect);

    QPen my_pen = QPen(Qt::red);        // Draw lines with a red pen
    scene->addLine(QLineF(rect.bottomLeft(), rect.bottomRight()) ,my_pen);
    scene->addLine(QLineF(rect.topLeft(), rect.topRight()) ,my_pen);
    scene->addLine(QLineF(rect.bottomLeft(), rect.topLeft()) ,my_pen);
    scene->addLine(QLineF(rect.topRight(), rect.bottomRight()) ,my_pen);

}

MainWindow::~MainWindow()
{
   /* QFile save_file("pvz_players.csv");
    if (save_file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
     QTextStream out(&save_file);
     out <<
     for(int i = 0; i<limit;i++)
     {
         out <<  << ":" << player[i][1];
     }
     // Save data. E.g.: out << timestamp << ":" << name << ":" << lvl << "\n"
     save_file.close();
    }*/

    delete ui;
}

void MainWindow::setLevel()
{
    if(currentUser>=0)
    {
        currentLevel = player[currentUser].level;
    }
    QPixmap levelPix = ("c:/Qt/final_project/level-"+QString::number(currentLevel)+".png");
    ui->levelLabel->setPixmap(levelPix);
}

int MainWindow::loadLevels()
{
    QFile f("c:/Qt/final_project/pvz_levels.csv");
    if(!f.exists()) //check if file exists
    {
        fileExistsDialog();
        return 0;
    }

    else
    {
        if(!f.open(QIODevice::ReadOnly)){ //check if the same file is open
        return(0);
        }
        QString allData = f.readAll();
        QList<QString> firstSplit = allData.split("\r\n", QString::SkipEmptyParts);

        for(int i=0; i<firstSplit.size();i++)
        {
        levelData[i] = firstSplit[i].split(",");

        zombieSeq[i] = levelData[i][1].split(":");

        levelData[i][1].clear();
        }
    }
    return(1);
}

void MainWindow::loadUsers()
{
    QFile f("c:/Qt/final_project/pvz_users.csv");
    if(!f.exists()) //check if file exists
    {
        return;
    }

    else
    {
        if(!f.open(QIODevice::ReadOnly)){ //check if the same file is open
        return;
        }
        QString allData = f.readAll();
        QList<QString> firstSplit = allData.split("\r\n", QString::SkipEmptyParts);


        if(firstSplit.size() < 5) //check when to stop player parsing
            limit = firstSplit.size();

        for(int i=0; i<limit;i++)
        {
        userData[i] = firstSplit[i].split(",");
        }

        for(int i=0; i<limit; i++)
        {
            if(userData[i][2].toInt() > 100 || !checkAlpha(userData[i][2]))
            {
                return;
            }

            if(userData[i][1].length() > 10 || checkAlphaNum(userData[i][1]))
            {
                return;
            }

            player[i].timestamp = userData[i][0].toInt();
            player[i].level = userData[i][2].toInt();
            player[i].name = userData[i][1];
            ui->userButton->addItem(userData[i][1],limit-i);
        }
    }
    return;
}

void MainWindow::fileExistsDialog()
{
    QMessageBox mDialog;
    mDialog.setText("Error: Cannot find level data.");
    mDialog.exec();
}

int MainWindow::checkAlpha(QString string)
{
    int len = string.length();
    for(int i = 0; i<len; i++)
    {
    if(!string[i].isLetter())
            return(1);
    }
    return(0);
}

int MainWindow::checkAlphaNum(QString string)
{
    int len = string.length();
    for(int i = 0; i<len; i++)
    {
        if(!string[i].isLetterOrNumber())
            return (1);
    }
    return (0);
}

void MainWindow::on_userButton_currentIndexChanged(int index)
{
    currentUser = index;
    setLevel();
}

void MainWindow::on_deleteButton_clicked()
{
    if (currentUser>=0)
    {
        QMessageBox msgBox;
        msgBox.setText("Delete user");
        msgBox.setInformativeText("Are you sure you want to delete current user?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Yes: //Yes, delete current user
              for(int i=currentUser;i<limit;i++)
              {
              player[i].level = player[i+1].level;
              player[i].name = player[i+1].name;
              player[i].timestamp = player[i+1].timestamp;
              }
              ui->userButton->removeItem(currentUser);
              limit = limit-1;
              currentLevel = player[currentUser].level;
              loadLevels();
              break;
          case QMessageBox::No: //No, retain current user
              break;
          default:
              // should never be reached
              break;
        }
    }
}
