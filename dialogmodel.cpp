#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include "Config/modelitem.h"
#include "Config/configholder.h"
#include "Config/texture.h"
#include "Config/textureimg.h"
#include "Config/texturetxt.h"
#include <QDebug>
#include <QFileDialog>
#include <QImageReader>
#include <QPainter>

DialogModel::DialogModel(QWidget *parent, canvaItem *item, Canva *c) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);

    connect(ui->ModelList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemActivated1(QListWidgetItem*)));
    connect(ui->pbAddPage,SIGNAL(pressed()),this,SLOT(buttonPlus()));
    connect(ui->pbRemovePage,SIGNAL(pressed()),this,SLOT(buttonMoins()));
    connect(ui->pbSaveZone,SIGNAL(pressed()),this,SLOT(modelEnregistrement()));
    connect(ui->TextureList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemActivated(QListWidgetItem*)));
    connect(ui->pbAddZone,SIGNAL(pressed()),this,SLOT(buttonPlus1()));
    connect(ui->pbRemoveZone,SIGNAL(pressed()),this,SLOT(buttonMoins1()));
    connect(ui->cbTextureType, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->pbPathIMG, SIGNAL(released()),this,SLOT(openFile()));
    connect(ui->pbPathMOV, SIGNAL(released()),this,SLOT(openFile2()));
    connect(ui->teText,SIGNAL(textChanged()),this,SLOT(changetext()));

    canva = c;

    int i=1;
    QVector<Model*> items = canva->getItems();
    foreach (Model* m, items) {
       ui->ModelList->addItem("Zone " + QString::number(i++));
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
    } else {
        displayedHeight = ui->lbpixmap->height();
        displayedWidth =  ui->lbpixmap->width() * canva->getPix().width()/canva->getPix().height();

        ui->lbpixmap->setFixedHeight(displayedHeight);
        ui->lbpixmap->setFixedWidth(displayedWidth);
        ui->lbpixmap->setPixmap(canva->getPix().scaled(displayedWidth,ui->lbpixmap->maximumHeight(),Qt::KeepAspectRatio));
    }
    ui->lbpixmap->setGeometry(geo);
    ratioX = (double) 100.0/displayedWidth;
    ratioY = (double) 100.0/displayedHeight;

    ui->stackedWidget->setEnabled(false);
    ui->gbText->setEnabled(false);

    ui->widgetSelect->raise();
}



DialogModel::~DialogModel()
{
    this->releaseMouse();
    delete ui;
}

void DialogModel::on_buttonBox_accepted()
{
    this->hide();
}

void DialogModel::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une image"),"/",tr("Image Files (*.png  *.jpg *.bmp *.jpeg)"));

    if (fileName != NULL){

        int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
        model->getTextures().remove(pos_to_suppress);

        QImageReader *reader = new QImageReader();
        reader->setFileName(fileName);
        QImage image =reader->read();
        QPixmap map=QPixmap::fromImage(image);
        TextureIMG* tImage = new TextureIMG(map);
        tImage->setLocalPath(fileName);

        model->getTextures().insert(pos_to_suppress,tImage);
        ui->lePathIMG->insert(fileName);
        model->setModified(true);

        //Load pixmap
        ui->widgetSelect->getLabel()->setPixmap(map);
        ui->widgetSelect->getLabel()->setVisible(true);
    }
}

void DialogModel::openFile2()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une vidéo"),"/",tr("Image Files (*.mp4 )"));
    ui->lePathMOV->insert(fileName);

}

void DialogModel::buttonPlus()
{
    Texture *t = new TextureTXT("");
    model->addTexture(t);

    ui->TextureList->clear();

    QVector<Texture*> items = model->getTextures();
    int i = 1;
    foreach (Texture* t, items) {
       ui->TextureList->addItem("Texture " + QString::number(i++));
    }
}

void DialogModel::buttonMoins(){

    if(ui->TextureList->selectedItems().size()<1) return;
    int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
    model->getTextures().remove(pos_to_suppress);
    ui->TextureList->clear();
    QVector<Texture*> items = model->getTextures();
    int i = 1;
    foreach (Texture* m, items) {
       ui->TextureList->addItem("Texture " + QString::number(i++));
    }
    if(model->getTextures().size()<1) {
        ui->cbTextureType->setEnabled(false);
        ui->stackedWidget->setEnabled(false);
        ui->gbText->setEnabled(false);
        ui->pbSaveZone->setEnabled(false);
        ui->gbModele->setEnabled(true);
        ui->buttonBox->setEnabled(true);
        return;
    } else {
        ui->TextureList->setCurrentRow(model->getTextures().size()-1);
    }
}

//On click texture
void DialogModel::itemActivated(QListWidgetItem* i){

    ui->cbTextureType->setEnabled(true);
    ui->stackedWidget->setEnabled(true);
    ui->pbSaveZone->setEnabled(true);
    ui->widgetSelect->getLabel()->setVisible(true);
    ui->gbModele->setEnabled(false);

    int pos = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
    if (model->getTextures().value(pos)->getType() == Texture::TEXT){
        Texture* t = model->getTextures().value(pos);
        TextureTXT* test = (TextureTXT*) t;
        QString textTexture = test->getData();
        qDebug() << "Texte texture = " << textTexture << endl;
        ui->teText->setText(textTexture);
        ui->stackedWidget->setCurrentIndex(0);
        ui->cbTextureType->setCurrentIndex(0);

        ui->widgetSelect->getLabel()->setText(textTexture);
    } else if(model->getTextures().value(pos)->getType() == Texture::IMG) {
        Texture* t = model->getTextures().value(pos);
        TextureIMG* test = (TextureIMG*) t;
        QString filename = test->getLocalPath();
        qDebug() << "FILENAME texture = " << filename << endl;
        QImageReader *reader = new QImageReader();
        reader->setFileName(filename);
        QImage image =reader->read();
        QPixmap map=QPixmap::fromImage(image);
        Texture* tImage = new TextureIMG(map);

        ui->lePathIMG->insert(filename);

        ui->stackedWidget->setCurrentIndex(1);
        ui->cbTextureType->setCurrentIndex(1);
        ui->widgetSelect->getLabel()->setPixmap(map);
        ui->widgetSelect->getLabel()->setVisible(true);
    }

}

void DialogModel::buttonPlus1(){
    ui->pbRemoveZone->setEnabled(true);
    QString name = "Nouveau";
    Model *m = new Model(name,canva->getItems().size());
    canva->addModel(m);
    ui->ModelList->clear();

    int i = 1;
    QVector<Model*> items = canva->getItems();
    foreach (Model* m, items) {
       ui->ModelList->addItem("Zone " + QString::number(i++));
    }
}

void DialogModel::buttonMoins1(){

    if(ui->ModelList->selectedItems().size()<1) return;

       int pos_to_suppress = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();
       canva->getItems().remove(pos_to_suppress);
       ui->ModelList->clear();

       int i = 1;
       QVector<Model*> items = canva->getItems();
       foreach (Model* m, items) {
          ui->ModelList->addItem("Zone " + QString::number(i++));
       }

       if(items.size()<1) {
           ui->cbTextureType->setEnabled(false);
           ui->stackedWidget->setEnabled(false);
           ui->gbText->setEnabled(false);
           ui->pbSaveZone->setEnabled(false);
           ui->gbModele->setEnabled(true);
           ui->buttonBox->setEnabled(true);
           ui->pbRemoveZone->setEnabled(false);
           return;
       } else {
           ui->TextureList->setCurrentRow(items.size()-1);
       }

}
//On click model
void DialogModel::itemActivated1(QListWidgetItem* i){

    ui->cbTextureType->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    ui->pbSaveZone->setEnabled(false);
     ui->widgetSelect->getLabel()->setVisible(false);
    ui->gbText->setEnabled(true);




    if(ui->ModelList->selectedItems().size()<1) return;
    int pos = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();
    model = canva->getItems().value(pos);

    ui->TextureList->clear();

    int indTex=0;
    foreach(Texture *ti, model->getTextures()){
        qDebug() << "Texture " + QString::number(indTex);
        ui->TextureList->addItem("Texture " + QString::number(++indTex));
    }

    //Load Rectangle
    if( !model->tlc.isEmpty() && !model->trc.isEmpty() && !model->blc.isEmpty() && !model->brc.isEmpty()) {
        qDebug() << "LOAD RECTANGLE ";
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

        brcX += (ui->lbpixmap->x()) ;
        brcY += (ui->lbpixmap->y() + ui->lbpixmap->height());
        ui->widgetSelect->getRubberBand()->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        ui->widgetSelect->getLabel()->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        ui->widgetSelect->getRubberBand()->show();
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }

}

void DialogModel::changetext(){

    QString text = ui->teText->toPlainText();
    TextureTXT * ttext = new TextureTXT(text);

    ui->widgetSelect->getLabel()->setVisible(true);
    ui->widgetSelect->getLabel()->setText(text);

    int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
    model->getTextures().remove(pos_to_suppress);
    model->getTextures().insert(pos_to_suppress,ttext);
    model->setModified(true);
}

void DialogModel::modelEnregistrement(){

    WidgetSelection *widget = ui->widgetSelect;
    QRect rect = widget->getRectSelection();

    qDebug() << "rectangle" << rect.height() << endl;
    qDebug() << "rectangle" << rect.width() << endl;
    qDebug() << "rectangle" << rect.isEmpty() << endl;
    QString text ;



    if(rect.height() !=30 && rect.width() != 100){
        qDebug() << "Valid" << endl;
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
        model->tlc = QString("").append((QString::number(confTLCx))).append(",").append(QString::number(-confTLCy)).append(",0");
        model->trc = QString("").append((QString::number(confTRCx))).append(",").append(QString::number(-confTRCy)).append(",0");
        model->blc = QString("").append((QString::number(confBLCx))).append(",").append(QString::number(-confBLCy)).append(",0");
        model->brc = QString("").append((QString::number(confBRCx))).append(",").append(QString::number(-confBRCy)).append(",0");
        //TextureIMG *timg = new TextureIMG(canva->getPix());
        //model->addTexture(timg);

        ui->gbModele->setEnabled(true);
        ui->buttonBox->setEnabled(true);

    } else {
        qDebug() << "Selection Nulle" << endl;

    }

}
