#include "widgetSelection.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <iostream>
#include <Qt>
#include <QGraphicsView>
#include <QGraphicsOpacityEffect>

WidgetSelection::WidgetSelection(QWidget *parent)
    : QWidget(parent)
{
    toto = parent;

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    move_rubberBand=false;
    selection_start=false;

    labelWid = new QLabel(this);
    labelWid->setVisible(false);
    labelWid->setScaledContents(true);
    labelWid->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

WidgetSelection::~WidgetSelection()
{

}


void WidgetSelection::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "MousePressEvent : "<< e << endl;

   if (e->button()==Qt::LeftButton)
   {
        if (rubberBand->geometry().contains(e->pos())){
            move_rubberBand = true ;
            rubberBand_offset = e->pos() - rubberBand->pos();
        }
        else{
            selection_start = true;
            origin = e->pos();
            //rubberBand->show();
        }
   }
}

void WidgetSelection::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "MouseMoveEnvent : "<< e << endl;

    if(move_rubberBand) {
        rubberBand->move(e->pos() - rubberBand_offset);
        labelWid->move(e->pos() - rubberBand_offset);
     } else
        if(selection_start){
            rubberBand->setGeometry(QRect(origin,e->pos()));
            labelWid->setGeometry(QRect(origin,e->pos()));
            rubberBand->show();
        }
}

void WidgetSelection::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "MouseReleaseEvent : "<< e << endl;
    qDebug() << "rectangle :" << rubberBand->geometry() << endl;

    labelWid->setGeometry(rubberBand->geometry());
    move_rubberBand = false;
    selection_start = false;
    rubberBand->show();
}

QRect WidgetSelection::getRectSelection(){
    return rubberBand->geometry();
}

QRubberBand* WidgetSelection::getRubberBand() {
    return rubberBand;
}

QLabel* WidgetSelection::getLabel(){
    return labelWid;
}


