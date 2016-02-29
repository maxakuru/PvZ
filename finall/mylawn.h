#ifndef MYLAWN_H
#define MYLAWN_H

#include <QWidget>
#include <QGraphicsView>

class MyLawn : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyLawn(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // MYLAWN_H
