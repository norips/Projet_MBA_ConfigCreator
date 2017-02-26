#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QMenu>
#include "mainwindow.h"



class MainWindow;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent);
    ~Widget();

public:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QRect getRectSelection();



private:
    QWidget *toto;

    bool selectionStarted;
    QRect selectionRect;
    QMenu contextMenu;

    QImage image;
    QPixmap imageobject;

    int newPenWidth;
    QColor newPenColor;
    bool modified;
    bool scribbling;

    QPoint firstPoint, secondPoint;
    void drawFirstPoint(const QPoint);
    void drawSecondPoint(const QPoint);

public slots:
    void saveSlot();


};

#endif // WIDGET_H
