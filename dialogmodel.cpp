#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include "Config/modelitem.h"
#include "Config/configholder.h"
#include "Config/texture.h"
#include "Config/textureimg.h"
#include "Config/texturetxt.h"
#include <QDebug>
#include <QFileDialog>
#include <QPainter>

DialogModel::DialogModel(QWidget *parent,modelItem* m, Canva *c) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);
    model = m->getModel();

    ui->leName->setText(m->getName());

    canva = c;

    ui->lbpixmap->setPixmap(canva->getPix().scaled(ui->lbpixmap->rect().size(),Qt::KeepAspectRatio));

    foreach(Texture *t, model->getTextures()){
            /*if((model->getTextures().at(0))->getType() == Texture::IMG && model->getTextures().size() > 0) {
                TextureIMG *timg = dynamic_cast<TextureIMG *>(model->getTextures().at(0));
                ui->lbTexture->setPixmap(timg->getData());
            }
        else*/
            if(t->getType() == Texture::TEXT){
                TextureTXT *ttext = dynamic_cast<TextureTXT*>(t);
                ui->teText->setText(ttext->getData());
            }
    }

}



DialogModel::~DialogModel()
{
    this->releaseMouse();
    //delete widget;
    delete ui;
}

void DialogModel::on_buttonBox_accepted()
{
    Widget *widget = ui->widget;
    QRect rect = widget->getRectSelection();
    qDebug() << "rectangle" << rect << endl;
    QString text ;


    if(rect.height() !=0 && rect.width() != 0){
        qDebug() << "Valid" << endl;
        text = ui->lbText->text();
        int x =0, y=0, width, height;
        rect.getRect(&x,&y,&width,&height);
        QPoint xtlc(x ,  y);
        QPoint xtrc(x+width,  y);
        QPoint xblc(x, y+height);
        QPoint xbrc(x + width,y+height);
        int lbX=0, lbY=0, lbWidth, lbHeight;
        ui->lbpixmap->geometry().getRect(&lbX,&lbY,&lbWidth,&lbHeight);
        lbWidth = ui->lbpixmap->width();
        lbHeight = ui->lbpixmap->height();

        bool landscape = canva->getPix().width() > canva->getPix().height();

        float displayedHeight;
        float displayedWidth;

        if (landscape){
            displayedHeight = ui->lbpixmap->height() * canva->getPix().height()/canva->getPix().width();
            displayedWidth =  ui->lbpixmap->width();
        } else {
            displayedHeight = ui->lbpixmap->height();
            displayedWidth =  ui->lbpixmap->width() * canva->getPix().width()/canva->getPix().height();
        }
        double ratioX = (double) 100.0/displayedWidth;
        double ratioY = (double) 100.0/displayedHeight;

        QPoint tlc(lbX,lbY);
        QPoint trc(lbX+lbWidth,lbY);
        QPoint blc(lbX,lbY+lbHeight);
        QPoint brc(lbX+lbWidth,lbY+lbHeight);


        qDebug() << "tlc : " << xtlc-blc << endl;
        qDebug() << "trc : " << xtrc-blc << endl;
        qDebug() << "blc : " << xblc-blc << endl;
        qDebug() << "brc : " << xbrc-blc << endl;
        QPoint relTLC = xtlc-blc;
        QPoint relTRC = xtrc-blc;
        QPoint relBLC = xblc-blc;
        QPoint relBRC = xbrc-blc;
        double confTLCx = (double) relTLC.x() * ratioX;
        double confTLCy = (double) relTLC.y() * ratioY;
        double confTRCx = (double) relTRC.x() * ratioX;
        double confTRCy = (double) relTRC.y() * ratioY;
        double confBLCx = (double) relBLC.x() * ratioX;
        double confBLCy = (double) relBLC.y() * ratioY;
        double confBRCx = (double) relBRC.x() * ratioX;
        double confBRCy = (double) relBRC.y() * ratioY;
        qDebug() << "######CONFIG VALUE#########" << endl;
        qDebug() << "RATIO X : " << ratioX << "," << ",RATIO Y :" << ratioY << endl;
        qDebug() << "TLC : " << confTLCx << "," << confTLCy << endl;
        qDebug() << "TRC : " << confTRCx << "," << confTRCy << endl;
        qDebug() << "BLC : " << confBLCx << "," << confBLCy << endl;
        qDebug() << "BRC : " << confBRCx << "," << confBRCy << endl;
        model->tlc.append((QString::number(confTLCx))).append(",").append(QString::number(-confTLCy)).append(",0");
        model->trc.append((QString::number(confTRCx))).append(",").append(QString::number(-confTRCy)).append(",0");
        model->blc.append((QString::number(confBLCx))).append(",").append(QString::number(-confBLCy)).append(",0");
        model->brc.append((QString::number(confBRCx))).append(",").append(QString::number(-confBRCy)).append(",0");
        model->name = ui->leName->text();
        TextureTXT * ttext = new TextureTXT(text);
        model->addTexture(ttext);
        //TextureIMG *timg = new TextureIMG(canva->getPix());
        //model->addTexture(timg);
        this->hide();
    } else {
        qDebug() << "Selection Nulle" << endl;
    }
    //this->releaseMouse();
    this->hide();
}
