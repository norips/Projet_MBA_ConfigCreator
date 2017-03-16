#include "widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <iostream>
#include <Qt>
#include <QGraphicsView>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    toto = parent;

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    move_rubberBand=false;
}

Widget::~Widget()
{

}


void Widget::mousePressEvent(QMouseEvent *e)
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

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << "MouseMoveEnvent : "<< e << endl;

    if(move_rubberBand)
        rubberBand->move(e->pos() - rubberBand_offset);
    else
        if(selection_start){
            rubberBand->setGeometry(QRect(origin,e->pos()));
            rubberBand->show();
        }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "MouseReleaseEvent : "<< e << endl;
    qDebug() << "rectangle :" << rubberBand->geometry() << endl;

    move_rubberBand = false;
    selection_start = false;
    rubberBand->show();
}

QRect Widget::getRectSelection(){
    return rubberBand->geometry();
}

QRubberBand* Widget::getRubberBand() {
    return rubberBand;
}
