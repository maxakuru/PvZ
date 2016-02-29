#ifndef MYLAWN_H
#define MYLAWN_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <qdebug.h>
//#include "mainwindow.h"

class MyLawn : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyLawn(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    QPointF getClickPoint() const;
    QPointF clickpt;
    QList<QRectF> getSunptRect();

signals:

public slots:

private:

};

#endif // MYLAWN_H
