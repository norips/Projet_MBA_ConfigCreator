#include "preview.h"
#include "ui_preview.h"
#include <QPixmap>
#include <QRect>


Preview::Preview(QWidget *parent, Canva *c) :
    QDialog(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);

    canva = c;

    bool landscape = canva->getPix().width() > canva->getPix().height();
    float displayedHeight;
    float displayedWidth;

    QRect geo = ui->tableau->geometry();
    if (landscape){
        displayedHeight = ui->tableau->height() * canva->getPix().height()/canva->getPix().width();
        displayedWidth =  ui->tableau->width();

        ui->tableau->setFixedHeight(displayedHeight);
        ui->tableau->setFixedWidth(displayedWidth);
        ui->tableau->setPixmap(canva->getPix().scaled(ui->tableau->maximumWidth(),displayedHeight,Qt::KeepAspectRatio));
        geo.translate(0,displayedHeight);
    } else {
        displayedHeight = ui->tableau->height();
        displayedWidth =  ui->tableau->width() * canva->getPix().width()/canva->getPix().height();

        ui->tableau->setFixedHeight(displayedHeight);
        ui->tableau->setFixedWidth(displayedWidth);
        ui->tableau->setPixmap(canva->getPix().scaled(displayedWidth,ui->tableau->maximumHeight(),Qt::KeepAspectRatio));
    }
    ui->tableau->setGeometry(geo);
    ratioX = (double) 100.0/displayedWidth;
    ratioY = (double) 100.0/displayedHeight;

    QVector<Model*> items = canva->getItems();
    for(int i = 0; i < items.size();i++) {
       create_pixmap(canva->getItems().value(i));
    }

}

Preview::~Preview()
{
    delete ui;
}

void Preview::create_pixmap(Model *m)
{
    //TODO
    //creation des pixmaps correspondant aux modèles


}
