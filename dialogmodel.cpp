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
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->pushButton, SIGNAL(released()),this,SLOT(openFile()));
    connect(ui->pushButton_2, SIGNAL(released()),this,SLOT(openFile2()));
    ui->leName->setText(m->getName());

    canva = c;


    foreach(Texture *t, model->getTextures()){
        if((model->getTextures().at(0))->getType() == Texture::IMG && model->getTextures().size() > 0) {
                TextureIMG *timg = dynamic_cast<TextureIMG *>(model->getTextures().at(0));
                ui->lbText->setPixmap(timg->getData());
            }
        else
            if(t->getType() == Texture::TEXT){
                TextureTXT *ttext = dynamic_cast<TextureTXT*>(t);
                ui->teText->setText(ttext->getData());
                break;
            }
    }
    bool landscape = canva->getPix().width() > canva->getPix().height();

    float displayedHeight;
    float displayedWidth;

    QRect geo = ui->lbpixmap->geometry();
    if (landscape){
        displayedHeight = ui->lbpixmap->height() * canva->getPix().height()/canva->getPix().width();
        displayedWidth =  ui->lbpixmap->width();

        ui->lbpixmap->setFixedHeight(displayedHeight);
        ui->lbpixmap->setFixedWidth(displayedWidth);
        ui->lbpixmap->setPixmap(canva->getPix().scaled(ui->lbpixmap->maximumWidth(),displayedHeight,Qt::KeepAspectRatio));
        geo.translate(0,displayedHeight);
        qDebug() << "Land";
    } else {
        displayedHeight = ui->lbpixmap->height();
        displayedWidth =  ui->lbpixmap->width() * canva->getPix().width()/canva->getPix().height();

        ui->lbpixmap->setFixedHeight(displayedHeight);
        ui->lbpixmap->setFixedWidth(displayedWidth);
        ui->lbpixmap->setPixmap(canva->getPix().scaled(displayedWidth,ui->lbpixmap->maximumHeight(),Qt::KeepAspectRatio));
        geo.translate(displayedWidth/2,0);
    }
    ui->lbpixmap->setGeometry(geo);

    qDebug()<< "width = " << ui->lbpixmap->width() << " height = " << ui->lbpixmap->height() << endl;
    ratioX = (double) 100.0/displayedWidth;
    ratioY = (double) 100.0/displayedHeight;

    //Load it
    if( !model->tlc.isEmpty() && !model->trc.isEmpty() && !model->blc.isEmpty() && !model->brc.isEmpty()) {
        QStringList lTLC = model->tlc.split(",");
        double tlcX = lTLC.at(0).toDouble() / ratioX;
        double tlcY = lTLC.at(1).toDouble() / ratioY;
        tlcY = -tlcY;

        QStringList lTRC = model->trc.split(",");
       // double trcX = lTRC.at(0).toDouble() / ratioX;
        double trcY = lTRC.at(1).toDouble() / ratioY;
        trcY = -trcY;

        QStringList lBLC = model->blc.split(",");
        //double blcX = lBLC.at(0).toDouble() / ratioX;
        double blcY = lBLC.at(1).toDouble() / ratioY;
        blcY = -blcY;

        QStringList lBRC = model->brc.split(",");
        double brcX = lBRC.at(0).toDouble() / ratioX;
        double brcY = lBRC.at(1).toDouble() / ratioY;
        brcY = -brcY;

        tlcX += (ui->lbpixmap->x() ) ;
        tlcY += (ui->lbpixmap->y() + ui->lbpixmap->height()) ;
        qDebug() << tlcX << "," << tlcY << endl;
        qDebug() << brcX << "," << brcY << endl;

        brcX += (ui->lbpixmap->x() ) ;
        brcY += (ui->lbpixmap->y() + ui->lbpixmap->height());
        ui->widget->getRubberBand()->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        ui->widget->getRubberBand()->show();

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
        text = ui->teText->toPlainText();
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
    this->hide();
}

void DialogModel::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une image"),"/",tr("Image Files (*.png *.jpg *.bmp *.jpeg"));
    ui->lineEdit->insert(fileName);
    // t.setLocalPath(fileName);

    qDebug() << "FILE" << fileName;
}

void DialogModel::openFile2()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une vidéo"),"/",tr("Image Files (*.mp4 )"));
    ui->lineEdit_2->insert(fileName);
    // t.setLocalPath(fileName);

    qDebug() << "FILE" << fileName;
}

