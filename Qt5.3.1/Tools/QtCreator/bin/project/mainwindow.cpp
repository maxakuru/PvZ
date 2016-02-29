#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //initialize variables
    currentUser = -1; //no current user
    currentLevel = 0;
    limit = 5; //player limit(cannot be more than 5)
    currentPoints = 5000;
    isPainted = 0;
    activePlant = 0;
    activeRows.push_back(2); //middle row initially added to active rows
    loopcount = 0;
    zombieTimer = 20; //level one has 20 second zombie delay
    zombieCount = 0; //no zombies yet appeared
    zombiesKilled = false;

    for(int i=0;i<5;i++)
        zombiesInRow[i] = 0;

    for(int i=0; i<8; i++)
    {
        active[i] = true;
        seedingTime[i] = 0;
    }

    ui->setupUi(this);

    lawn = this->findChild<MyLawn*>("graphicsView");

    int check = loadLevels();
    if(check==0)
        QApplication::quit(); // set up error message here
    loadUsers();

    //connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));

    scene = new QGraphicsScene(this);   // scene holds all objects in the scene
    ui->graphicsView->setScene(scene);  // graphicsView is the viewport on to the scene
    qDebug() << scene;
    recto = (ui->graphicsView->geometry());
    double xp = recto.x();
    double yp = recto.y();
    recto.setTop(0);
    recto.setBottom(recto.height()-(yp+5));
    recto.setLeft(0);
    recto.setRight(recto.width()-(xp+5));
    scene->setSceneRect(recto);

    QBrush brush(QColor(85,61,24));
    scene->setBackgroundBrush(brush);

    columnWidth = recto.width()/10;
    rowHeight = recto.height()/5;

    QPen my_pen = QPen(Qt::black);        // Draw lines with a black pen
    scene->addLine(QLineF(recto.bottomLeft(), recto.bottomRight()) ,my_pen);
    scene->addLine(QLineF(recto.topLeft(), recto.topRight()) ,my_pen);
    scene->addLine(QLineF(recto.bottomLeft(), recto.topLeft()) ,my_pen);
    scene->addLine(QLineF(recto.topRight(), recto.bottomRight()) ,my_pen);



    //paint lawn, add mowers
    paintLawn();

    QSize buttonSize(50,50);

    //set button images
    ui->p1_Button->setIcon(QIcon(":/misc/p1"));
    ui->p1_Button->setIconSize(buttonSize);
    ui->p1_Button->setText("100");
    ui->p1_Button->setDisabled(true);

    ui->p2_Button->setIcon(QIcon(":/misc/p2"));
    ui->p2_Button->setIconSize(buttonSize);
    ui->p2_Button->setText("50");
    ui->p2_Button->setDisabled(true);

    ui->p3_Button->setIcon(QIcon(":/misc/p3"));
    ui->p3_Button->setIconSize(buttonSize);
    ui->p3_Button->setText("150");
    ui->p3_Button->setDisabled(true);

    ui->p4_Button->setIcon(QIcon(":/misc/p4"));
    ui->p4_Button->setIconSize(buttonSize);
    ui->p4_Button->setText("50");
    ui->p4_Button->setDisabled(true);

    ui->p5_Button->setIcon(QIcon(":/misc/p5"));
    ui->p5_Button->setIconSize(buttonSize);
    ui->p5_Button->setText("25");
    ui->p5_Button->setDisabled(true);

    ui->p6_Button->setIcon(QIcon(":/misc/p6"));
    ui->p6_Button->setIconSize(buttonSize);
    ui->p6_Button->setText("175");
    ui->p6_Button->setDisabled(true);

    ui->p7_Button->setIcon(QIcon(":/misc/p7"));
    ui->p7_Button->setIconSize(buttonSize);
    ui->p7_Button->setText("150");
    ui->p7_Button->setDisabled(true);

    ui->p8_Button->setIcon(QIcon(":/misc/p8"));
    ui->p8_Button->setIconSize(buttonSize);
    ui->p8_Button->setText("200");
    ui->p8_Button->setDisabled(true);

    //disable some buttons
    ui->quitButton->setDisabled(true);
    ui->restartButton->setDisabled(true);

    //lawn->setMainWindow(this);
    lastClickPoint = QPointF(0,0);

    //set zombie timer
    setZombieTimer();
}

MainWindow::~MainWindow()
{
    if(currentUser > 0)
        players.at(currentUser)->timestamp = QDateTime::currentDateTime().toTime_t();

    QFile save_file("C:\\Qt\\Qt5.3.1\\Tools\\QtCreator\\bin\\project\\misc\\pvz_players2.csv");
    if (save_file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
     QTextStream out(&save_file);

     out.setCodec("UTF-8");

     for(int i = 0; i<players.size();i++)
     {
         out << players.at(i)->timestamp << ":" << players.at(i)->name << ":" << players.at(i)->level << "\n";
     }

     QFile old_file("C:\\Qt\\Qt5.3.1\\Tools\\QtCreator\\bin\\project\\misc\\pvz_players.csv");
     old_file.remove();

     save_file.close();
    }

    QFile("C:\\Qt\\Qt5.3.1\\Tools\\QtCreator\\bin\\project\\misc\\pvz_players2.csv").rename("C:\\Qt\\Qt5.3.1\\Tools\\QtCreator\\bin\\project\\misc\\pvz_players2.csv","C:\\Qt\\Qt5.3.1\\Tools\\QtCreator\\bin\\project\\misc\\pvz_players.csv");

    delete ui;
}

void MainWindow::setLevel()
{
    //if(currentUser>0)
    {
    //empty active rows vector
    activeRows.clear();

    if(currentUser>=0 && players.at(currentUser)->level >= 0)
    {
        currentLevel = players.at(currentUser)->level;

        if(currentLevel > 0)
        {
            QPixmap levelPix = (":/misc/project/misc/level-"+QString::number(currentLevel)+".png");
            ui->levelLabel->setPixmap(levelPix);
        }

        else
            ui->levelLabel->clear();
    }

    if(currentLevel==1)
        activeRows.push_back(2);

    else if(currentLevel==2)
    {
        activeRows.push_back(1);
        activeRows.push_back(2);
        activeRows.push_back(3);
    }

    else if(currentLevel>=3)
    {
        activeRows.push_back(0);
        activeRows.push_back(1);
        activeRows.push_back(2);
        activeRows.push_back(3);
        activeRows.push_back(4);
    }

    rePaintLawn();

    //set zombie timer
    if(currentLevel>0)
        zombieTimer = levelData[currentLevel-1][3].toDouble();
    }
}

int MainWindow::loadLevels()
{
    QFile f(":/misc/pvz_levels");
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
        levelData[i] = firstSplit[i].split(":");

        zombieSeq[i] = levelData[i][1].split(",");

        levelData[i][1].clear();
        }
    }

    //set zombie timer
    if(currentLevel > 0)
        zombieTimer = levelData[currentLevel-1][3].toDouble();

    return(1);
}

void MainWindow::loadUsers()
{
    QFile f(":/misc/pvz_players");
    if(!f.exists()) //check if file exists
    {
        ui->startButton->setDisabled(true);
        limit = 0;
        return;
    }

    else
    {
        if(!f.open(QIODevice::ReadOnly)){ //check if the same file is open
        return;
        }

         bool throwAway = false;
         QList<QString> firstSplit;

        QTextStream in(&f);
               while ( !in.atEnd() )
               {
                   QByteArray line = f.readLine();
                   line.chop(2);
                   QString liney = QString(line);
                   int size = liney.size()-1;
                   int checkchar = liney.at(size).digitValue();
                   if(checkchar < 0 || checkchar > 9)
                       throwAway = true;

                   firstSplit.append(liney);
               }

        QByteArray encodedString = f.readAll();
        //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        //QString string = codec->toUnicode(encodedString);
       QString string = QString(encodedString);

        //QString allData = f.readAll();
        //QList<QString> firstSplit = string.split("\r\n", QString::SkipEmptyParts);

        if(firstSplit.size() < 5) //check when to stop player parsing
            limit = firstSplit.size();

        for(int i=0; i<limit;i++)
        {
        userData[i] = firstSplit[i].split(":");
        }

        for(int i = 0; i < limit; i++)
        {
            if(userData[i][2].toInt() > 100 || !checkAlpha(userData[i][2]) || userData[i][2].toInt() < 0 || userData[i][2].isEmpty())
            {
                throwAway = true;
            }

            if(userData[i][1].length() > 10 || userData[i][1].contains(QRegExp("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")))
            {
                throwAway = true;
            }
        }

        if(throwAway == true)
        {
            QMessageBox mDialog;
            mDialog.setText("Warning: Corrupt player data.");
            mDialog.exec();
            ui->startButton->setDisabled(true);
            ui->deleteButton->setDisabled(true);
            limit = 0;
        }

        if(throwAway == false)
        {
            ui->userButton->setDisabled(false);

        for(int i=0; i<limit; i++)
        {
            UserClass *u = new UserClass(userData[i][0].toInt(),userData[i][2].toInt(),userData[i][1]);
            //user1.level = userData[i][2].toInt();
            //user1.name = userData[i][1];
            //user1.timestamp = userData[i][0].toInt();
            //player[i].timestamp = userData[i][0].toInt();
            //player[i].level = userData[i][2].toInt();
            //player[i].name = userData[i][1];
            //ui->userButton->addItem(userData[i][1],i);
            if(!players.isEmpty())
            {
            if(u->timestamp >= players.first()->timestamp)
                players.prepend(u);
            else
                players.append(u);
            }
            else
                players.append(u);

            //ui->userButton->addItem(players.at(i)->name,limit-i);
        }

        //add users to drop down list
        for(int z = 0; z<limit; z++)
        {
            //qDebug() << players.at(z)->name;
            ui->userButton->addItem(players.at(z)->name, z);
        }
        }
    }
    return;
}

void MainWindow::fileExistsDialog()
{
    QMessageBox mDialog;
    mDialog.setText("Error: Cannot find level data.");
    mDialog.exec();
    QApplication::quit();
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

void MainWindow::paintLawn()
{
    QBrush brush(Qt::darkGreen);
    QBrush brownBrush(QColor(85,61,24));
    QPen my_pen(Qt::white);
    //add cells
    for(int x=0;x<10;x++)
    {
        for(int y=0;y<5;y++)
        {
            //cell[x][y](columnWidth*x,rowHeight*y,columnWidth,rowHeight);
            //cell[x][y]->setRect(columnWidth*x,rowHeight*y,columnWidth,rowHeight);
            rect[x][y].setTop(rowHeight*y);
            rect[x][y].setHeight(rowHeight);
            rect[x][y].setLeft(columnWidth*x);
            rect[x][y].setWidth(columnWidth);
            cell[x][y].setRect(rect[x][y]);
            cell[x][y].setPen(my_pen);

            if(x==0) //outline home column with white
            {
                cell[x][y].setPen(my_pen);
                scene->addItem(&cell[x][y]);
                //c.push_back(cell[x][y]);
            }

            if(currentLevel==0)
            {
                cell[x][y].setBrush(brownBrush);
                cell[x][y].setPen(my_pen);
                scene->addItem(&cell[x][y]);
            }

            else if(currentLevel==1&&y==2&&x>0) //if level is 1, only paint middle row green
            {
                cell[x][y].setBrush(brush);
                cell[x][y].setPen(my_pen);
                scene->addItem(&cell[x][y]);
                //c.push_back(cell[x][y]);
            }

            else if(currentLevel==1&&y!=2&&x>0) //paint others brown
            {
                cell[x][y].setBrush(brownBrush);
                cell[x][y].setPen(my_pen);
                scene->addItem(&cell[x][y]);
                //c.push_back(cell[x][y]);
            }

            else if(currentLevel==2&&x>0&&(y==1||y==2||y==3)) //if level is 2 paint middle 3 rows green
            {
                cell[x][y].setBrush(brush);
                scene->addItem(&cell[x][y]);
            }

            else if(currentLevel==2&&x>0&&(y==0||y==4)) //paint others brown
            {
                cell[x][y].setBrush(brownBrush);
                scene->addItem(&cell[x][y]);
            }

            else if(currentLevel>2&&x>0) //if level is greater than 2 paint all green
            {
                cell[x][y].setBrush(brush);

                scene->addItem(&cell[x][y]);
                //c.push_back(cell[x][y]);
            }
        }
    }

    if(isPainted==0)
    {
        for(int i=0; i<5; i++)
        {
            Mower *m = new Mower(i);
            scene->addItem(m);
            mowers.push_back(m);
        }

    /*QPixmap pixmap(":/misc/lawnmower");

    for(int i=0;i<5;i++)
    {
        //mower[i] = scene->addItem(&mowerPix);
        mower[i] = scene->addPixmap(pixmap);
        mower[i]->setPos(rect[0][i].topLeft());
    }*/
    }

    isPainted++;
}

void MainWindow::rePaintLawn()
{
    if(currentLevel==0)
    {
        for(int y=0;y<5;y++)
        {
            for(int x=1;x<10;x++)
            {
                cell[x][y].setBrush(QBrush(QColor(85,61,24)));
            }

        }
    }

   else if(currentLevel==1)
    {
        for(int y=0;y<5;y++)
        {
            for(int x=1;x<10;x++)
            {
                if(y!=2)
                {
                   cell[x][y].setBrush(QBrush(QColor(85,61,24)));

                }

                else if(y==2)
                {
                    cell[x][y].setBrush(QBrush(Qt::darkGreen));
                }
            }

        }
    }

    else if(currentLevel==2)
    {
       for(int y=0;y<5;y++)
       {
           for(int x=1;x<10;x++)
           {
               if(y==0||y==4)
               {
                  cell[x][y].setBrush(QBrush(QColor(85,61,24)));
               }

               else if(y!=0||y!=4)
               {
                   cell[x][y].setBrush(QBrush(Qt::darkGreen));
               }
           }

       }

    }

    else if(currentLevel>2)
    {
       for(int y=0;y<5;y++)
       {
           for(int x=1;x<10;x++)
           {
               cell[x][y].setBrush(QBrush(Qt::darkGreen));
           }

       }
    }

   ui->graphicsView->repaint();
}


void MainWindow::drawPlant()
{
    if(activePlant>0&&clickPoint!=lastClickPoint)
    {
        if(clickPoint.x()>columnWidth)
        {
            //if plant points is not empty
            if(!plants.isEmpty())
            {
                for(int y=activeRows.first();y<=activeRows.last();y++)
                {
                        for(int x=1;x<10;x++)
                        {
                            if(cell[x][y].contains(clickPoint)&&cont()&&activePlant<8)
                            {
                                if(activePlant==1)
                                {
                                    Peashooter *p = new Peashooter(1, 4, 7.5, 9999, 1, 1.5, QPixmap(":/misc/p1"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p); //add plant to plant vector
                                    peaPt.push_back(clickPoint);
                                    plantPt.push_back(clickPoint); //add clickpoint to plantpt vector
                                    shooterPlants.push_back(p);
                                    currentPoints -= 100;
                                    activePlant = 0;
                                    seedingTime[0] = p->seeding;
                                    break;
                                }

                                else if(activePlant==2)
                                {
                                    Sunflower *p = new Sunflower(2, 4, 7.5, 24, QPixmap(":/misc/p2"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    sunflowers.push_back(p); //add sunflower to sunflowers vector
                                    qDebug() << clickPoint;
                                    currentPoints -= 50;
                                    activePlant = 0;
                                    seedingTime[1] = p->seeding;
                                    break;
                                }

                                else if(activePlant==3)
                                {
                                    Cherry *p = new Cherry(3, 9999, 50, 1, 90, 1, 1, 3, QPixmap(":/misc/p3"), cell[x][y].boundingRect(), y, x);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    crPlants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    qDebug() << clickPoint;
                                    currentPoints -= 150;
                                    activePlant = 0;
                                    seedingTime[2] = p->seeding;
                                }

                                else if(activePlant==4)
                                {
                                    WallNut *p = new WallNut(4, 72, 30, QPixmap(":/misc/p4"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    qDebug() << clickPoint;
                                    currentPoints -= 50;
                                    activePlant = 0;
                                    seedingTime[3] = p->seeding;
                                }

                                else if(activePlant==5)
                                {
                                    Potato *p = new Potato(5, 4, 30, 1, 90, 15, 1, QPixmap(":/misc/p5"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    crPlants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    qDebug() << clickPoint;
                                    currentPoints -= 25;
                                    activePlant = 0;
                                    seedingTime[4] = p->seeding;
                                }

                                else if(activePlant==6) //snowpea
                                {
                                    Snowpea *p = new Snowpea(6, 4, 7.5, 9999, 1, 1.5, 1, QPixmap(":/misc/p6"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    shooterPlants.push_back(p);
                                    qDebug() << clickPoint;
                                    currentPoints -= 175;
                                    activePlant = 0;
                                    seedingTime[5] = p->seeding;
                                }

                                else if(activePlant==7) //chomper
                                {
                                    Peashooter *p = new Peashooter(7, 4, 7.5, 1, 150, 42, QPixmap(":/misc/p7"), cell[x][y].boundingRect(), y);
                                    scene->addItem(p);
                                    plants.push_back(p);
                                    crPlants.push_back(p);
                                    plantPt.push_back(clickPoint);
                                    qDebug() << clickPoint;
                                    currentPoints -= 150;
                                    activePlant = 0;
                                    seedingTime[6] = p->seeding;
                                }

                            }

                            else if(cell[x][y].contains(clickPoint)&&activePlant==8&&othercont())
                            {
                                removePeaShooter(clickPoint);
                                Repeater *p = new Repeater(8, 4, 7.5, 9999, 2, 1.5, 1, QPixmap(":/misc/p8"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                plantPt.push_back(clickPoint);
                                qDebug() << clickPoint;
                                shooterPlants.push_back(p);
                                currentPoints -= 200;
                                activePlant = 0;
                                seedingTime[7] = p->seeding;
                                active[0] = true;
                            }
                        }
                }
            }


            //if plant points is empty
            else
            {
                for(int x=0;x<10;x++)
                {
                    for(int y=0;y<5;y++)
                    {
                        if(cell[x][y].contains(clickPoint)&&activeRows.contains(y))
                        {
                            if(activePlant==1)
                            {
                                Peashooter *p = new Peashooter(1, 4, 7.5, 9999, 1, 1.5, QPixmap(":/misc/p1"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                peaPt.push_back(clickPoint);
                                plantPt.push_back(clickPoint);
                                shooterPlants.push_back(p);
                                qDebug() << clickPoint;
                                currentPoints -= 100;
                                activePlant = 0;
                                seedingTime[0] = p->seeding;
                                break;
                            }

                            else if(activePlant==2)
                            {
                                Sunflower *p = new Sunflower(2, 4, 7.5, 24, QPixmap(":/misc/p2"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                plantPt.push_back(clickPoint);
                                sunflowers.push_back(p);
                                qDebug() << clickPoint;
                                currentPoints -= 50;
                                activePlant = 0;
                                seedingTime[1] = p->seeding;
                                break;
                            }

                            else if(activePlant==3)
                            {
                                Cherry *p = new Cherry(3, 9999, 50, 1, 90, 1, 1, 3, QPixmap(":/misc/p3"), cell[x][y].boundingRect(), y, x);
                                scene->addItem(p);
                                plants.push_back(p);
                                crPlants.push_back(p);
                                plantPt.push_back(clickPoint);
                                qDebug() << clickPoint;
                                currentPoints -= 150;
                                activePlant = 0;
                                seedingTime[2] = p->seeding;
                            }

                            else if(activePlant==4)
                            {
                                WallNut *p = new WallNut(4, 72, 30, QPixmap(":/misc/p4"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                plantPt.push_back(clickPoint);
                                qDebug() << clickPoint;
                                currentPoints -= 50;
                                activePlant = 0;
                                seedingTime[3] = p->seeding;
                            }

                            else if(activePlant==5)
                            {
                                Potato *p = new Potato(5, 4, 30, 1, 90, 15, 1, QPixmap(":/misc/p5"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                crPlants.push_back(p);
                                plantPt.push_back(clickPoint);
                                qDebug() << clickPoint;
                                currentPoints -= 25;
                                activePlant = 0;
                                seedingTime[4] = p->seeding;
                            }

                            else if(activePlant==6) //snowpea
                            {
                                Snowpea *p = new Snowpea(6, 4, 7.5, 9999, 1, 1.5, 1, QPixmap(":/misc/p6"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                plantPt.push_back(clickPoint);
                                shooterPlants.push_back(p);
                                qDebug() << clickPoint;
                                currentPoints -= 175;
                                activePlant = 0;
                                seedingTime[5] = p->seeding;
                            }

                            else if(activePlant==7) //chomper
                            {
                                Peashooter *p = new Peashooter(7, 4, 7.5, 1, 150, 42, QPixmap(":/misc/p7"), cell[x][y].boundingRect(), y);
                                scene->addItem(p);
                                plants.push_back(p);
                                crPlants.push_back(p);
                                plantPt.push_back(clickPoint);
                                qDebug() << clickPoint;
                                currentPoints -= 150;
                                activePlant = 0;
                                seedingTime[6] = p->seeding;
                            }

                            else if(activePlant==8)
                            {
                                ui->p8_Button->setDisabled(false);
                            }
                        }
                    }
                }
            }
            }
        }

    lastClickPoint = clickPoint;
}

void MainWindow::addSunpoints()
{
    //create new sunpoint
    Sunpoint *s = new Sunpoint;
    scene->addItem(s);

    //add sunpoint to sunpoint vector
    sunpts.push_back(s);

    //add sunpoint's bounding rect to rect vector
    sunptRects.push_back(s->boundingRect());
}

void MainWindow::addSunflowerPoints(QRectF brect)
{
    //create new sunpoint
    Sunpoint *s = new Sunpoint(brect);
    scene->addItem(s);

    //add sunpoint to sunpoint vector
    sunpts.push_back(s);

    //add sunpoint's bounding rect to rect vector
    sunptRects.push_back(s->boundingRect());
}

void MainWindow::updateZombies()
{
    QList<int> removeThese;

    for(int i = 0; i < zombieVector.size(); i++)
    {
        //regular or flag zombie
        if(zombieVector.at(i)->number <= 2)
        {
            if(zombieVector.at(i)->life <= 0)
                removeThese.prepend(i);
        }

        //conehead or buckethead zombie
        else if(zombieVector.at(i)->number == 3 || zombieVector.at(i)->number == 4)
        {
            //if life of conehead or buckethead is zero, give them second life value and remove equipment
            if(zombieVector.at(i)->life <= 0 && zombieVector.at(i)->life2 > 0)
            {
                zombieVector.at(i)->life = zombieVector.at(i)->life2;
                zombieVector.at(i)->life2 = 0;
                zombieVector.at(i)->image = QPixmap(":/misc/regzomb");
                scene->removeItem(zombieVector.at(i));
                scene->addItem(zombieVector.at(i));

            }

            else if(zombieVector.at(i)->life <= 0 && zombieVector.at(i)->life2 <= 0)
            {
                removeThese.prepend(i);
            }
        }

        //newspaper zombie
        else if(zombieVector.at(i)->number == 5)
        {
            //if life is used up, give zombie life2 and change speed to speed2, take away equipment
            if(zombieVector.at(i)->life <= 0 && zombieVector.at(i)->life2 > 0)
            {
                zombieVector.at(i)->life = zombieVector.at(i)->life2;
                zombieVector.at(i)->life2 = 0;
                zombieVector.at(i)->speed = zombieVector.at(i)->speed2;
                zombieVector.at(i)->image = QPixmap(":/misc/regzomb");
                scene->addItem(zombieVector.at(i));
            }
        }
    }

    //remove removeThese from scene and zombievector
    for (int j = 0; j < removeThese.size(); j++)
    {
        zombiesKilled = true;
        zombiesInRow[zombieVector.at(removeThese.at(j))->randomY] -= 1;
        scene->removeItem(zombieVector.at(removeThese.at(j)));
        zombieVector.removeAt(removeThese.at(j));
    }
}

void MainWindow::checkSunPointClick(QPointF point)
{
    for(int i = 0; i < sunpts.size();i++)
    {
        if(sunpts.at(i)->currentRect.contains(point))
        {
            currentPoints += 25;
            scene->removeItem(sunpts.at(i));
            sunpts.remove(i);
        }

    }
}

bool MainWindow::cont()
{
    for(int y = activeRows.first(); y <= activeRows.last(); y++)
    {
        for(int x = 1; x<10; x++)
        {
            for(int f=0;f<plantPt.size();f++)
            {
                if(cell[x][y].contains(clickPoint))
                {
                    if(cell[x][y].contains(plantPt.at(f)))
                        return false;
                }
            }
        }
    }

    return true;
}

bool MainWindow::othercont()
{
    for(int y = activeRows.first(); y <= activeRows.last(); y++)
    {
        for(int x = 1; x<10; x++)
        {
            for(int f=0;f<peaPt.size();f++)
            {
                if(cell[x][y].contains(clickPoint))
                {
                    if(cell[x][y].contains(peaPt.at(f)))
                        return true;
                }
            }
        }
    }

    return false;
}

void MainWindow::updateSunflowers()
{
    for(int i = 0; i < sunflowers.size(); i++)
    {
        qDebug() << "sunflower " << i << ", seconds: " << sunflowers.at(i)->seconds;
        //increase seconds count
        sunflowers.at(i)->seconds += 0.1;

        //check if seconds matches rate
        if(sunflowers.at(i)->seconds >= sunflowers.at(i)->rate)
        {
            //add sunflower point
            addSunflowerPoints(sunflowers.at(i)->boundingRect());

            //reset seconds counter
            sunflowers.at(i)->seconds = 0;
        }
    }
}

bool MainWindow::checkPlantExists(int number) //returns true if plant number exists
{
    for(int i = 0; i<plants.size(); i++)
    {
        if(plants.at(i)->number == number)
            return true;
    }

    return false;
}

void MainWindow::checkSunpointAge()
{

    for(int i=0; i<sunpts.size(); i++)
    {
        //if seconds counter reaches zero, delete sunpoint without adding points
        if(sunpts.at(i)->seconds<=0)
        {
            scene->removeItem(sunpts.at(i));
            sunpts.remove(i);
        }
        else //count down 10 milliseconds
            sunpts.at(i)->seconds -= 0.1;
    }
}

void MainWindow::setZombieTimer()
{
    if(currentLevel == 1)
        zombieTimer = 20;

    else if(currentLevel == 2)
        zombieTimer = 15;
}

void MainWindow::spawnZombie()
{
    //if current spawning zombie is type 1
    if(zombieSeq[currentLevel-1][zombieCount].toInt()==1)
    {
        Zombie *z = new Zombie(1, 10, 1, 0.5, 5.0, QPixmap(":/misc/regzomb"), QPixmap(":/misc/regzombfight"), activeRows, zombiesInRow);
        zombieVector.push_back(z);
        scene->addItem(z);
    }

    //current spawning zombie is type 2
    else if(zombieSeq[currentLevel-1][zombieCount].toInt()==2)
    {
        Zombie *z = new Zombie(2, 10, 1, 0.5, 3.75, QPixmap(":/misc/flagzomb"), QPixmap(":/misc/flagzombfight"), activeRows, zombiesInRow);
        zombieVector.push_back(z);
        scene->addItem(z);
    }

    //current spawning zombie is type 3
    else if(zombieSeq[currentLevel-1][zombieCount].toInt()==3)
    {
        ZombieTwo *z = new ZombieTwo(3, 18, 10, 1, 0.5, 5.0, QPixmap(":/misc/conezomb"), QPixmap(":/misc/conezombfight"), activeRows, zombiesInRow);
        zombieVector.push_back(z);
        scene->addItem(z);
    }

    //current spawning zombie is type 4
    else if(zombieSeq[currentLevel-1][zombieCount].toInt()==4)
    {
        ZombieTwo *z = new ZombieTwo(4, 55, 10, 1, 0.5, 5.0, QPixmap(":/misc/bucketzomb"), QPixmap(":/misc/bucketzombfight"), activeRows, zombiesInRow);
        zombieVector.push_back(z);
        scene->addItem(z);
    }

    //current spawning zombie is type 5
    else if(zombieSeq[currentLevel][zombieCount].toInt()==5)
    {
        PaperZombie(5, 8, 8, 1, 0.5, 5.0, 2.0, QPixmap(":/misc/newspaperzomb"), QPixmap(":/misc/newspaperzombfight"), activeRows, zombiesInRow);
    }

    //add one to zombie count
    zombieCount += 1;

    //reset zombietimer
    zombieTimer = levelData[currentLevel-1][4].toDouble()-(levelData[currentLevel-1][5].toDouble()*(zombieCount-1));
}

void MainWindow::clearVectors()
{
    if(!plantPt.isEmpty())
        plantPt.clear();

    if(!plants.isEmpty())
    {
        qDeleteAll(plants);
        plants.clear();
    }

    if(!peaPt.isEmpty())
        peaPt.clear();


    if(!crPlants.isEmpty())
    {
        //qDeleteAll(crPlants);
        crPlants.clear();
    }

    if(!shooterPlants.isEmpty())
    {
        //qDeleteAll(shooterPlants);
        shooterPlants.clear();
    }

    if(!bullets.isEmpty())
    {
        qDeleteAll(bullets);
        bullets.clear();
    }

    if(!zombieVector.isEmpty())
    {
        qDeleteAll(zombieVector);
        zombieVector.clear();
    }

    if(!sunflowers.isEmpty())
    {
        //qDeleteAll(sunflowers);
        sunflowers.clear();
    }

    if(!sunpts.isEmpty())
    {
        qDeleteAll(sunpts);
        sunpts.clear();
    }

    if(!sunptRects.isEmpty())
    {
        sunptRects.clear();
    }

    for(int i=0;i<5;i++)
        zombiesInRow[i] = 0;

    for(int i = 0; i < 8; i++)
        active[i] = true;
}

void MainWindow::checkShooters()
{
    for(int i = 0; i<shooterPlants.size(); i++)
    {
        if(shooterPlants.at(i)->seconds >= shooterPlants.at(i)->rate && zombiesInRow[shooterPlants.at(i)->row] > 0)
        {
            //determine type of bullet to be created
            int type = 0;

            QRectF bulletRect;
            int Damage;

            //plant number 1
            if(shooterPlants.at(i)->number == 1)
            {
                type = 1;
                Damage = 1;
                bulletRect = QRectF(shooterPlants.at(i)->boundingRect().x()+70,shooterPlants.at(i)->boundingRect().y()+25,17,17);
            }

            else if(shooterPlants.at(i)->number == 8)
            {
                type = 1;
                Damage = 2;
                bulletRect = QRectF(shooterPlants.at(i)->boundingRect().x()+70,shooterPlants.at(i)->boundingRect().y()+25,17,17);
            }

            else if(shooterPlants.at(i)->number == 6)
            {
                type = 2;
                Damage = 1;
                bulletRect = QRectF(shooterPlants.at(i)->boundingRect().x()+70,shooterPlants.at(i)->boundingRect().y()+35,17,17);
            }

            //create new bullet
            Bullet *b = new Bullet(type, bulletRect, Damage);
            bullets.push_back(b);
            scene->addItem(b);

            //reset seconds counter
            shooterPlants.at(i)->seconds = 0;
        }

        else
            shooterPlants.at(i)->seconds += 0.1;
    }
}

void MainWindow::updateButtons()
{
    //check whether

    //update points label with current points
    ui->pointsLabel->setNum(currentPoints);

    // 25 <= points < 50
    if(currentPoints < 50 && currentPoints >=25)
    {
        ui->p1_Button->setDisabled(true);
        ui->p2_Button->setDisabled(true);
        ui->p3_Button->setDisabled(true);
        ui->p4_Button->setDisabled(true);
        if(active[4])
            ui->p5_Button->setDisabled(false);
        ui->p6_Button->setDisabled(true);
        ui->p7_Button->setDisabled(true);
        ui->p8_Button->setDisabled(true);
    }

    //50 <= points < 100
    if(currentPoints < 100 && currentPoints >=50)
    {
        ui->p1_Button->setDisabled(true);
        if(active[1])
            ui->p2_Button->setDisabled(false);
        ui->p3_Button->setDisabled(true);
        if(active[3])
            ui->p4_Button->setDisabled(false);
        if(active[4])
            ui->p5_Button->setDisabled(false);
        ui->p6_Button->setDisabled(true);
        ui->p7_Button->setDisabled(true);
        ui->p8_Button->setDisabled(true);
    }

    //100 <= points < 150
    if(currentPoints < 150 && currentPoints >=100)
    {
        if(active[0])
            ui->p1_Button->setDisabled(false);
        if(active[1])
            ui->p2_Button->setDisabled(false);
        ui->p3_Button->setDisabled(true);
        if(active[3])
            ui->p4_Button->setDisabled(false);
        if(active[4])
            ui->p5_Button->setDisabled(false);
        ui->p6_Button->setDisabled(true);
        ui->p7_Button->setDisabled(true);
        ui->p8_Button->setDisabled(true);
    }

    //150 <= points < 175
    if(currentPoints < 175 && currentPoints >=150)
    {
        if(active[0])
            ui->p1_Button->setDisabled(false);

        if(active[1])
            ui->p2_Button->setDisabled(false);

        if(active[2])
            ui->p3_Button->setDisabled(false);

        if(active[3])
            ui->p4_Button->setDisabled(false);

        if(active[4])
            ui->p5_Button->setDisabled(false);

        ui->p6_Button->setDisabled(true);

        if(active[6])
            ui->p7_Button->setDisabled(false);

        ui->p8_Button->setDisabled(true);
    }

    //175 <= points < 200
    if(currentPoints < 200 && currentPoints >=175)
    {
        if(active[0])
            ui->p1_Button->setDisabled(false);

        if(active[1])
            ui->p2_Button->setDisabled(false);

        if(active[2])
            ui->p3_Button->setDisabled(false);

        if(active[3])
            ui->p4_Button->setDisabled(false);

        if(active[4])
            ui->p5_Button->setDisabled(false);

        if(active[5])
            ui->p6_Button->setDisabled(false);

        if(active[6])
            ui->p7_Button->setDisabled(false);

        ui->p8_Button->setDisabled(true);
    }

    //points >= 200
    if(currentPoints >= 200)
    {
        if(active[0])
            ui->p1_Button->setDisabled(false);

        if(active[1])
            ui->p2_Button->setDisabled(false);

        if(active[2])
            ui->p3_Button->setDisabled(false);

        if(active[3])
            ui->p4_Button->setDisabled(false);

        if(active[4])
            ui->p5_Button->setDisabled(false);

        if(active[5])
            ui->p6_Button->setDisabled(false);

        if(active[6])
            ui->p7_Button->setDisabled(false);

        if(active[7])
            ui->p8_Button->setDisabled(false);
    }

    if(activePlant>0)
    {
        if(activePlant == 1)
            ui->p1_Button->setDisabled(true);

        else if(activePlant == 2)
            ui->p2_Button->setDisabled(true);

        else if(activePlant == 3)
            ui->p3_Button->setDisabled(true);

        else if(activePlant == 4)
            ui->p4_Button->setDisabled(true);

        else if(activePlant == 5)
            ui->p5_Button->setDisabled(true);

        else if(activePlant == 6)
            ui->p6_Button->setDisabled(true);

        else if(activePlant == 7)
            ui->p7_Button->setDisabled(true);

        else if(activePlant == 8)
            ui->p8_Button->setDisabled(true);
    }
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
              for(int i=currentUser;i<limit-1;i++)
              {
                players.at(i)->level = players.at(i+1)->level;
                players.at(i)->name = players.at(i+1)->name;
                players.at(i)->timestamp = players.at(i+1)->timestamp;
              }
              ui->userButton->removeItem(currentUser);
              limit = limit-1; //subtract 1 from number of current users
              currentLevel = players.at(currentUser)->level;
              setLevel();
              break;
          case QMessageBox::No: //No, retain current user
              break;
          default:
              // should never be reached
              break;
        }
    }
}

void MainWindow::on_p1_Button_clicked()
{
    if(currentPoints>=100)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 1;
    }
}

void MainWindow::on_p2_Button_clicked()
{
    if(currentPoints>=50)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 2;
    }
}

void MainWindow::on_p3_Button_clicked()
{
    if(currentPoints>=150)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 3;
    }
}

void MainWindow::on_p4_Button_clicked()
{
    if(currentPoints>=50)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 4;
    }
}

void MainWindow::on_p5_Button_clicked()
{
    if(currentPoints>=25)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 5;
    }
}

void MainWindow::on_p6_Button_clicked()
{
    if(currentPoints>=175)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 6;
    }
}

void MainWindow::on_p7_Button_clicked()
{
    if(currentPoints>=150)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 7;
    }
}

void MainWindow::on_p8_Button_clicked()
{
    if(currentPoints>=200)
    {
        ui->p1_Button->setDisabled(true);
        activePlant = 8;
    }
}

void MainWindow::update()
{    
    //check for collisions
    if(!bullets.isEmpty())
        checkBulletCollisions();

    //check zombie health, kill them if necessary
    if(!zombieVector.isEmpty())
        updateZombies();

    else
    {
        for(int i = 0; i < 5; i++)
            zombiesInRow[i] = 0;
    }

    //update active plant buttons and points display
    updateButtons();

    //sunpoint delay
    if(!loopcount%1000)
    {
        addSunpoints();
        loopcount = 80;
    }

    //set click point
    clickPoint = lawn->getClickPoint();

    qDebug() << "clickpoint: " << clickPoint;

    //check if click is on sunpoint
    if(clickPoint!=lastClickPoint)
            checkSunPointClick(clickPoint);

    //check sunpoint timers
    if(!sunpts.isEmpty())
    {
        checkSunpointAge();
    }

    //attempt drawing plant
    if(clickPoint!=lastClickPoint&&activePlant>0)
    {
        drawPlant();
    }

    //bring loopcount down one
    loopcount--;

    //set clickpoint to lastclickpoint
    lastClickPoint = clickPoint;

    //add 100 milliseconds to seconds counter
    secondsCounter += 0.1;

    //update sunflowers
    if(!sunflowers.isEmpty())
        updateSunflowers();

    //check seeding times
    if(!plants.isEmpty())
    {
        for(int i=0; i<8; i++)
        {
            if(checkPlantExists(i+1)&&seedingTime[i]>0)
            {
                active[i] = false;
                seedingTime[i] -= 0.1;
            }

            else if(checkPlantExists(i+1)&&seedingTime[i]<=0)
            {
                active[i] = true;
                seedingTime[i] = 0;
            }
        }
    }

    //check shooter plants for new bullets
    if(!shooterPlants.isEmpty())
        checkShooters();

    zombieTimer -= 0.05;

    if(zombieTimer <= 0&&zombieCount<zombieSeq[currentLevel-1].size())
    {
        spawnZombie();
    }

    //check close range plants
    if(!crPlants.isEmpty())
        checkCRPlants();

    if(!zombieVector.isEmpty())
        updateZombies();

    //check zombie attack status
    if(!zombieVector.isEmpty())
        checkZombieAttack();

    //check plant health
    if(!plants.isEmpty())
        updatePlants();

    //check if mower is touched by zombie
    if(!mowers.isEmpty())
        checkMowers();

    //check for mower collisions if there is one moving
    for(int i = 0; i<mowers.size(); i++)
    {
        if(mowers.at(i)->isMoving)
        {
            checkMowerCollisions();
        }
    }

    //check if a zombie has reached the home column
    if(mowers.size() < 5)
        checkGameOver();

    //move to next level
    bool cont = false;

    if(zombiesInRow[0] == 0 && zombiesInRow[1] == 0 && zombiesInRow[2] == 0 && zombiesInRow[3] == 0 && zombiesInRow[4] == 0)
        cont = true;

    if(zombieSeq[currentLevel-1].size() <= zombieCount && currentLevel < 8 && cont && zombiesKilled)
    {
        players.at(currentUser)->level++;
        currentLevel++;
        zombieCount = 0;
        zombiesKilled = false;
        zombieTimer = 0;
        setLevel();
        for(int i = 0; i < 8; i++)
            active[i] = true;
    }

    else if(zombieSeq[currentLevel-1].size() <= zombieCount && currentLevel == 8 && cont && zombiesKilled)
    {
        timer->stop();
        sunTimer->stop();

        QMessageBox msgBox;

        //move to middle
        msgBox.setGeometry(this->x()+300,this->y()+300,0,0);

        msgBox.setText("Winner winner chicken dinner");
        msgBox.setInformativeText("Congrats you win! Would you like to play again?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Ok: //ok, restart level
            zombieCount = 0;
            clearVectors();
            currentLevel = 1;
            setLevel();
            reAddMowers();
            sunTimer->start(100);
            timer->start(100);
            ui->startButton->setDisabled(true);
            ui->lineEdit->setDisabled(true);
            break;

          case QMessageBox::Cancel: //cancel, close game
            QApplication::quit();
              break;

          default:
              // should never be reached
              break;
        }
    }
}

void MainWindow::on_newButton_clicked()
{
    if(!checkAlphaNum(ui->lineEdit->text())&&!(ui->lineEdit->text().isEmpty())) //check that the name is valid
    {
    QMessageBox msgBox;
    msgBox.setText("New user");
    msgBox.setInformativeText(ui->lineEdit->text());
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Yes: //Yes, create new user
        if(limit<5)
        {
            UserClass *u = new UserClass(QDateTime::currentDateTime().toTime_t(), 1, ui->lineEdit->text());
            //player[limit].name = ui->lineEdit->text();
            //player[limit].level = 1;
            //player[limit].timestamp = QDateTime::currentDateTime().toTime_t();
            players.append(u);
            ui->userButton->addItem(players.at(limit)->name,limit+1);
            currentUser = limit;
            ui->userButton->setCurrentIndex(limit);
            setLevel();
            limit++;
            ui->lineEdit->clear();
        }

        else if(limit>=5)
        {
            for(int i=0;i<4;i++)
            {
              players.at(i)->level = players.at(i+1)->level;
              players.at(i)->name = players.at(i+1)->name;
              players.at(i)->timestamp = players.at(i+1)->timestamp;

              ui->userButton->setCurrentIndex(i);
              ui->userButton->setCurrentText(players.at(i)->name);
            }

            UserClass *u = new UserClass(QDateTime::currentDateTime().toTime_t(), 1, ui->lineEdit->text());

            ui->userButton->removeItem(4);

            players.removeAt(4);

            players.append(u);

            ui->userButton->addItem(players.at(4)->name,4);
            currentUser = 5;
            ui->userButton->setCurrentIndex(4);
            setLevel();
            ui->lineEdit->clear();
        }

        ui->startButton->setDisabled(false);
        ui->deleteButton->setDisabled(false);

          break;
      case QMessageBox::No: //No, do nothing
          break;
      default:
          // should never be reached
          break;
    }
    }
}

void MainWindow::checkBulletCollisions()
{
    //make a new list of items to remove, after loop remove them
    QList<int> removeItems;

    for(int j = 0; j < bullets.size(); j++)
    {
        //check if bullet is beyond scene
        if(bullets.at(j)->x() > 900)
            removeItems.prepend(j);

        for(int i = 0; i < zombieVector.size(); i++)
        {
            //bullet j collides with zombie i
            if(bullets.at(j)->collidingItems().contains(zombieVector.at(i)))
            {
                //add bullet to remove list
                if(!removeItems.contains(j))
                    removeItems.prepend(j);

                //remove health of zombie
                zombieVector.at(i)->life -= bullets.at(j)->damage;

                //check if bullet slows zombie
                if(bullets.at(j)->type == 2 && zombieVector.at(i)->speed >= 2.21)
                    zombieVector.at(i)->speed  -= 1;
            }
        }
    }

    //update zombie health, remove if necessary
    updateZombies();

    //remove removelist
    for(int p = 0; p < removeItems.size(); p++)
    {
        //from scene
        scene->removeItem(bullets.at(removeItems.at(p)));

        //then remove pointer
        bullets.removeAt(removeItems.at(p));
    }
}

void MainWindow::checkCRPlants()
{
    //items to remove
    QList<int> removeItems;

    for(int i=0; i<crPlants.size(); i++)
    {
        for(int j = 0; j < zombieVector.size(); j++)
        {
            //crplant i and zombie j are colliding
            if(crPlants.at(i)->collidingItems().contains(zombieVector.at(j)))
            {
                //if crPlant(i) is a bomb
                if(crPlants.at(i)->number == 3 || crPlants.at(i)->number == 5)
                {
                    //timer is up
                    if(crPlants.at(i)->seconds >= crPlants.at(i)->rate)
                    {
                        removeItems.prepend(i);

                        if(crPlants.at(i)->number == 5)
                            zombieVector.at(j)->life -= crPlants.at(i)->damage;
                    }

                    //otherwise count 0.1
                    else
                        crPlants.at(i)->seconds += 0.1;
                }

                //if crPlant(i) is chomper
                else if(crPlants.at(i)->number == 7)
                {
                    //timer is up
                    if(crPlants.at(i)->seconds >= crPlants.at(i)->rate)
                    {
                        zombieVector.at(j)->life -= crPlants.at(i)->damage;
                    }

                    //otherwise count 0.1
                    else
                        crPlants.at(i)->seconds += 0.1;
                }
            }
        }
    }

    //remove removelist
    for(int i = 0; i< removeItems.size(); i++)
    {
        //remove from scene
        scene->removeItem(crPlants.at(removeItems.at(i)));

        //reactivate plant
        active[3] = true;

        //remove plantpt
        deletePlantPt(crPlants.at(i)->boundingRect());

        //if crplant is a cherry bomb, apply damage to all zombies within 1 square
        if(crPlants.at(i)->number == 3)
        {
            //make list of zombies to damage
            QList<int> toDamage;

            //if cherry is in the middle of board
            if(crPlants.at(i)->column > 0 && crPlants.at(i)->column < 9 && crPlants.at(i)->row > 0 && crPlants.at(i)->row < 4)
            {

            for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
            {
                for(int y = crPlants.at(i)->row-1; y <= crPlants.at(i)->row+1; y++)
                {
                    for(int z = 0; z < zombieVector.size(); z++)
                    {
                        //if theyre within 3x3 cells, add to list
                        if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                        {
                            toDamage.prepend(z);
                        }
                    }
                }
            }
            }

            //at the right
            else if(crPlants.at(i)->column == 9 && crPlants.at(i)->row > 0 && crPlants.at(i)->row < 4)
            {
                for(int x = crPlants.at(i)->column-1;x < crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row-1; y <= crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the left
            else if(crPlants.at(i)->column == 1 && crPlants.at(i)->row > 0 && crPlants.at(i)->row < 4)
            {
                for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row-1; y <= crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the top
            else if(crPlants.at(i)->column < 9 && crPlants.at(i)->column > 1 && crPlants.at(i)->row == 0)
            {
                for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row; y <= crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the bottom
            else if(crPlants.at(i)->column < 9 && crPlants.at(i)->row == 4 && crPlants.at(i)->column > 1)
            {
                for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row-1; y < crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the top right
            else if(crPlants.at(i)->column == 9 && crPlants.at(i)->row == 0)
            {
                for(int x = crPlants.at(i)->column-1;x < crPlants.at(i)->column; x++)
                {
                    for(int y = crPlants.at(i)->row; y <= crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the top left
            else if(crPlants.at(i)->column == 1 && crPlants.at(i)->row == 0)
            {
                for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row; y <= crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the bottom left
            else if(crPlants.at(i)->column == 1 && crPlants.at(i)->row == 4)
            {
                for(int x = crPlants.at(i)->column-1;x <= crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row-1; y <= crPlants.at(i)->row; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //at the bottom right
            else if(crPlants.at(i)->column == 9 && crPlants.at(i)->row == 4)
            {
                for(int x = crPlants.at(i)->column-1;x < crPlants.at(i)->column+1; x++)
                {
                    for(int y = crPlants.at(i)->row-1; y < crPlants.at(i)->row+1; y++)
                    {
                        for(int z = 0; z < zombieVector.size(); z++)
                        {
                            //if theyre within 3x3 cells, add to list
                            if(cell[x][y].collidingItems().contains(zombieVector.at(z)))
                            {
                                toDamage.prepend(z);
                            }
                        }
                    }
                }
            }

            //damage zombie list
            for(int q = 0; q < toDamage.size(); q++)
            {
                zombieVector.at(toDamage.at(q))->life -= crPlants.at(i)->damage;
            }
        }

        //remove pointer item
        crPlants.removeAt(removeItems.at(i));
    }
}

void MainWindow::checkZombieAttack()
{
    if(plants.size() == 0)
    {
        for(int j = 0; j < zombieVector.size(); j++)
        {
            zombieVector.at(j)->isStopped = false;
        }
    }


    for(int i=0;i<plants.size();i++)
    {
        for(int j=0; j<zombieVector.size(); j++)
        {
            //zombie is already stopped
            if(zombieVector.at(j)->isStopped)
            {
                bool stillCol = false;

                //for loop to check if it's still colliding
                for(int z = 0; z< plants.size(); z++)
                {
                    //if it is colliding, bring down health
                    if(zombieVector.at(j)->collidingItems().contains(plants.at(z)))
                    {
                        //if zombie seconds >= rate, attack and reset seconds
                        if(zombieVector.at(j)->seconds >= zombieVector.at(j)->rate)
                        {
                            plants.at(z)->life -= zombieVector.at(j)->attack;
                            zombieVector.at(j)->seconds = 0;

                            if(plants.at(z)->life <= 0)
                                zombieVector.at(j)->isStopped = false;
                        }

                        //otherwise count 0.1
                        else
                            zombieVector.at(j)->seconds += 0.05;

                        stillCol = true;
                    }
                }

                //if it isnt colliding, start zombie again
                if(!stillCol)
                    zombieVector.at(j)->isStopped = false;
            }

            //zombie is not stopped, but is colliding with plant i, stop zombie
            else if(!zombieVector.at(j)->isStopped && zombieVector.at(j)->collidingItems().contains(plants.at(i)))
            {
                zombieVector.at(j)->isStopped = true;
            }
        }
    }
}

void MainWindow::updatePlants()
{
    QList<int> removeThese;
    QList<int> removeTheseSunflowers;
    QList<int> removeTheseCRPlants;
    QList<int> removeTheseShooters;

    for(int i = 0; i < plants.size(); i++)
    {
        if(plants.at(i)->life <= 0)
        {
            deletePlantPt(plants.at(i)->boundingRect());
            removeThese.prepend(i);

            if(plants.at(i)->number == 1)
                deletePeaPt(plants.at(i)->boundingRect());
        }
    }

    for(int i = 0; i < sunflowers.size(); i++)
    {
        if(sunflowers.at(i)->life <= 0)
            removeTheseSunflowers.prepend(i);
    }

    for(int i = 0; i < crPlants.size(); i++)
    {
        if(crPlants.at(i)->life <= 0)
            removeTheseCRPlants.prepend(i);
    }

    for(int i = 0; i < shooterPlants.size(); i++)
    {
        if(shooterPlants.at(i)->life <= 0)
            removeTheseShooters.prepend(i);
    }

    //remove from plants vector
    for(int i = 0; i < removeThese.size(); i++)
    {
        //remove from scene
        scene->removeItem(plants.at(removeThese.at(i)));

        //remove pointer
        plants.removeAt(removeThese.at(i));
    }

    //remove from sunflowers vector
    for(int i = 0; i < removeTheseSunflowers.size(); i++)
    {
        //remove from scene
        scene->removeItem(sunflowers.at(removeTheseSunflowers.at(i)));

        //remove pointer
        sunflowers.removeAt(removeTheseSunflowers.at(i));
    }

    //remove from crplant vector
    for(int i = 0; i < removeTheseCRPlants.size(); i++)
    {
        //remove from scene
        scene->removeItem(crPlants.at(removeTheseCRPlants.at(i)));

        //remove pointer
        crPlants.removeAt(removeTheseCRPlants.at(i));
    }

    //remove from shooterplant vector
    for(int i = 0; i < removeTheseShooters.size(); i++)
    {
        //remove from scene
        scene->removeItem(shooterPlants.at(removeTheseShooters.at(i)));

        //remove pointer
        shooterPlants.removeAt(removeTheseShooters.at(i));
    }
}

void MainWindow::deletePlantPt(QRectF rect)
{
    for(int i = 0; i < plantPt.size(); i++)
    {
        if(rect.contains(plantPt.at(i)))
            plantPt.removeAt(i);
    }
}

void MainWindow::deletePeaPt(QRectF rect)
{
    for(int i = 0; i < peaPt.size(); i++)
    {
        if(rect.contains(peaPt.at(i)))
            peaPt.removeAt(i);
    }
}

void MainWindow::removePeaShooter(QPointF point)
{
    for(int i = 0; i < plants.size(); i++)
    {
        if(plants.at(i)->boundingRect().contains(point))
        {
            deletePeaPt(plants.at(i)->boundingRect());
            deletePlantPt(plants.at(i)->boundingRect());
            scene->removeItem(plants.at(i));
            plants.removeAt(i);

        }
    }

    for(int i = 0; i < shooterPlants.size(); i++)
    {
        if(shooterPlants.at(i)->boundingRect().contains(point))
        {
            shooterPlants.removeAt(i);
        }
    }
}

void MainWindow::checkMowers()
{
    for(int i = 0; i < mowers.size(); i++)
    {
        for(int z = 0; z < zombieVector.size(); z++)
        {
            if(zombieVector.at(z)->collidingItems().contains(mowers.at(i)))
                mowers.at(i)->isMoving = true;
        }
    }

    QList<int> deleteThese;

    for(int i = 0; i < mowers.size(); i++)
    {
        if(mowers.at(i)->deleteMower == true)
            deleteThese.prepend(i);
    }

    for(int i = 0; i < deleteThese.size(); i++)
    {
        scene->removeItem(mowers.at(i));
        mowers.removeAt(i);
    }
}

void MainWindow::checkMowerCollisions()
{
    QList<int> removeThese;

    for(int z = 0; z < zombieVector.size(); z++)
    {
        for(int i = 0; i < mowers.size(); i++)
        {
            if(mowers.at(i)->isMoving && mowers.at(i)->collidingItems().contains(zombieVector.at(z)))
            {
                removeThese.prepend(z);
            }

        }
    }

    //remove the list from zombie vector
    for(int i = 0; i < removeThese.size(); i++)
    {
        zombiesKilled = true;
        zombiesInRow[zombieVector.at(removeThese.at(i))->randomY] -= 1;
        scene->removeItem(zombieVector.at(removeThese.at(i)));
        zombieVector.removeAt(removeThese.at(i));
    }
}

void MainWindow::reAddMowers()
{
    qDeleteAll(mowers);
    mowers.clear();

    for(int i=0; i<5; i++)
    {
        Mower *m = new Mower(i);
        scene->addItem(m);
        mowers.push_back(m);
    }
}

void MainWindow::checkGameOver()
{
    for(int i = 0; i < zombieVector.size(); i++)
    {
        if(zombieVector.at(i)->gameOver)
        {
            timer->stop();
            sunTimer->stop();

            QMessageBox msgBox;

            //move to middle
            msgBox.setGeometry(this->x()+300,this->y()+300,0,0);

            msgBox.setText("Game Over");
            msgBox.setInformativeText("Zombies have eaten your brains! /r/n Would you like to restart this level?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();

            switch (ret) {
              case QMessageBox::Ok: //ok, restart level
                zombieCount = 0;
                clearVectors();
                setLevel();
                reAddMowers();
                sunTimer->start(100);
                timer->start(100);
                ui->startButton->setDisabled(true);
                ui->lineEdit->setDisabled(true);
                break;

              case QMessageBox::Cancel: //cancel, close game
                QApplication::quit();
                  break;

              default:
                  // should never be reached
                  break;
            }
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    //set level to 1 if not set
    if(currentLevel == 0)
    {
        players.at(currentUser)->level++;
        currentLevel++;
        setLevel();
    }
    //enable and disable buttons
    updateButtons();
    ui->deleteButton->setDisabled(true);
    ui->restartButton->setDisabled(false);
    ui->startButton->setDisabled(true);
    ui->quitButton->setDisabled(false);
    ui->newButton->setDisabled(true);
    ui->userButton->setDisabled(true);

    //create timers
    timer = new QTimer(this);
    sunTimer = new QTimer(this);

    //connect timers to slots
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(sunTimer, SIGNAL(timeout()), scene, SLOT(advance()));

    //start timers
    timer->start(100);
    sunTimer->start(100);
}

void MainWindow::on_restartButton_clicked()
{
    //pause timers and game
    timer->stop();
    sunTimer->stop();

    QMessageBox msgBox;

    //move to middle
    QSpacerItem* horizontalSpacer = new QSpacerItem(610, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
    msgBox.setGeometry(this->x()+254,this->y()+275,0,0);

    msgBox.setText("Restart");
    msgBox.setInformativeText("Are you sure you want to restart?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    //change layout
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    layout->addItem(verticalSpacer, layout->columnCount(), 0, 1, layout->rowCount());

    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok: //ok, restart game
        zombieCount = 0;
        zombiesKilled = false;
        clearVectors();
        setLevel();
        reAddMowers();
        sunTimer->start(100);
        timer->start(100);
        ui->startButton->setDisabled(true);
        ui->lineEdit->setDisabled(true);
        break;

      case QMessageBox::Cancel: //cancel, start game as usual
        sunTimer->start(100);
        timer->start(100);
          break;

      default:
          // should never be reached
          break;
    }
}

void MainWindow::on_quitButton_clicked()
{
    //pause timers and game
    timer->stop();
    sunTimer->stop();

    QMessageBox msgBox;
    //move to middle
    QSpacerItem* horizontalSpacer = new QSpacerItem(610, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* verticalSpacer = new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
    msgBox.setGeometry(this->x()+254,this->y()+275,0,0);

    msgBox.setText("Quit");
    msgBox.setInformativeText("Are you sure you want to leave this level?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    //change layout
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    layout->addItem(verticalSpacer, layout->columnCount(), 0, 1, layout->rowCount());

    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok: //ok, quit level
        currentLevel = 1;
        currentPoints = 50;
        zombieCount = 0;
        zombiesKilled = false;
        clearVectors();
        setLevel();
        reAddMowers();
        ui->lineEdit->setDisabled(false);
        ui->restartButton->setDisabled(true);
        ui->deleteButton->setDisabled(false);
        ui->startButton->setDisabled(false);
        ui->quitButton->setDisabled(true);
        ui->userButton->setDisabled(false);
        ui->newButton->setDisabled(false);
        ui->p1_Button->setDisabled(true);
        ui->p2_Button->setDisabled(true);
        ui->p3_Button->setDisabled(true);
        ui->p4_Button->setDisabled(true);
        ui->p5_Button->setDisabled(true);
        ui->p6_Button->setDisabled(true);
        ui->p7_Button->setDisabled(true);
        ui->p8_Button->setDisabled(true);
        break;

      case QMessageBox::Cancel: //cancel, start game as usual
        sunTimer->start(100);
        timer->start(100);
          break;

      default:
          // should never be reached
          break;
    }
}
