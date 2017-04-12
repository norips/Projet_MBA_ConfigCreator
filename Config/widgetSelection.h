#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QMenu>
#include <QRubberBand>
#include <QVideoWidget>
#include <QMediaPlayer>
#include "../textedit.h"
#include "mainwindow.h"



class MainWindow;

class WidgetSelection : public QWidget
{
    Q_OBJECT

public:
    WidgetSelection(QWidget *parent);
    ~WidgetSelection();

public:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QRect getRectSelection();
    QRubberBand* getRubberBand();
    QLabel* getLabel();
    TextEdit* getTextEdit();
    QVideoWidget* getVideo();


private:
    QWidget *toto;
    QLabel *labelWid;
    TextEdit *teWid;
    QRubberBand* rubberBand;
    QVideoWidget *videoWidget;

    bool move_rubberBand;
    bool selection_start;
    QPoint rubberBand_offset;
    QPoint origin;

    QImage image;
    QPixmap imageobject;


};

#endif // WIDGET_H
