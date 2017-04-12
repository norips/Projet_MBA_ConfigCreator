#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include "Config/modelitem.h"
#include "Config/configholder.h"
#include "Config/texture.h"
#include "Config/textureimg.h"
#include "Config/texturetxt.h"
#include "Config/texturemov.h"

#include <QDebug>
#include <QFileDialog>
#include <QImageReader>
#include <QPainter>
#include <QMovie>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QVBoxLayout>
#include <QUrl>
#define UNUSED(x) (void)(x)
#define MAX_LOOP_TIMES 10
DialogModel::DialogModel(QWidget *parent, canvaItem *item, Canva *c) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);
    UNUSED(item);
    firstload = 1;
    connect(ui->ModelList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemActivated1(QListWidgetItem*)));
    connect(ui->pbAddPage,SIGNAL(pressed()),this,SLOT(buttonPlus()));
    connect(ui->pbRemovePage,SIGNAL(pressed()),this,SLOT(buttonMoins()));
    connect(ui->pbSaveZone,SIGNAL(pressed()),this,SLOT(modelEnregistrement()));
    connect(ui->TextureList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemActivated(QListWidgetItem*)));
    connect(ui->pbAddZone,SIGNAL(pressed()),this,SLOT(buttonPlus1()));
    connect(ui->pbRemoveZone,SIGNAL(pressed()),this,SLOT(buttonMoins1()));
    connect(ui->cbTextureType, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->pbPathIMG, SIGNAL(released()),this,SLOT(openFile()));
    connect(ui->pbPathMOV, SIGNAL(released()),this,SLOT(openFileVideo()));
    connect(ui->teText,SIGNAL(textChanged()),this,SLOT(changetext()));

    canva = c;
    QVector<Model*> items = canva->getItems();
    qDebug() << "Item size " << items.size();
    for(int i = 1; i < items.size()+1;i++) {
        ui->ModelList->addItem("Zone " + QString::number(i));
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
    QRect posWidg = ui->widgetSelect->geometry();
    posWidg.setHeight(1);
    posWidg.setWidth(1);
    ui->widgetSelect->getLabel()->setGeometry(posWidg);
    ui->widgetSelect->getTextEdit()->setGeometry(posWidg);
    ui->widgetSelect->getVideo()->setGeometry(posWidg);
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
        ui->widgetSelect->getVideo()->setVisible(false);
        ui->widgetSelect->getTextEdit()->setVisible(false);
    }
}

void DialogModel::openFileVideo()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir une vidéo"),"/",tr("Image Files (*.mp4)"));

    if (fileName != NULL){

        int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
        model->getTextures().remove(pos_to_suppress);
        TextureMOV* tMovie = new TextureMOV();
        tMovie->setLocalPath(fileName);

        /*IMAGE REPRESENTANT UNE VIDEO*
        QImageReader *reader = new QImageReader();
        reader->setFileName("../img/video.png");
        QImage image =reader->read();
        QPixmap map=QPixmap::fromImage(image);
        ui->widgetSelect->getLabel()->setPixmap(map);*/

        /*GIF *

        movie = new QMovie(fileName);
        ui->widgetSelect->getLabel()->setPixmap(map);
        ui->widgetSelect->getLabel()->setMovie(movie);
        movie->start();*/


        QMediaPlaylist *playlist = new QMediaPlaylist(this);
        playlist->addMedia(QUrl::fromLocalFile(fileName));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        QMediaPlayer *player = new QMediaPlayer(this);
        player->setPlaylist(playlist);

        player->setVideoOutput(ui->widgetSelect->getVideo());
        ui->widgetSelect->getVideo()->show();
        player->play();

        model->getTextures().insert(pos_to_suppress,tMovie);
        ui->lePathMOV->insert(fileName);
        model->setModified(true);


        //View Video
        ui->widgetSelect->getVideo()->setVisible(true);
        ui->widgetSelect->getLabel()->setVisible(false);
        ui->widgetSelect->getTextEdit()->setVisible(false);
    }
}


void DialogModel::buttonPlus()
{
    ui->gbModele->setEnabled(false);
    Texture *t = new TextureTXT("");
    model->addTexture(t);

    ui->TextureList->clear();

    QVector<Texture*> items = model->getTextures();
    for(int i = 1; i < items.size()+1; i++) {
        ui->TextureList->addItem("Page " + QString::number(i));
    }
    qDebug() << "SIZE ITEM" << items.size();

    if(items.size() == 1){
        ui->pbDownTexture->setEnabled(false);
        ui->pbUpTexture->setEnabled(false);
    }else{
        ui->pbDownTexture->setEnabled(true);
        ui->pbUpTexture->setEnabled(true);
    }
}


void DialogModel::buttonMoins(){

    if(ui->TextureList->selectedItems().size()<1) return;
    int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
    model->getTextures().remove(pos_to_suppress);
    ui->TextureList->clear();
    QVector<Texture*> items = model->getTextures();
    for(int i = 1; i < items.size()+1; i++) {
        ui->TextureList->addItem("Page " + QString::number(i));
    }
    if(model->getTextures().size()<1) {
        ui->cbTextureType->setEnabled(false);
        ui->stackedWidget->setEnabled(false);
        ui->gbText->setEnabled(false);
        ui->pbSaveZone->setEnabled(false);
        ui->gbModele->setEnabled(true);
        return;
    } else {
        ui->TextureList->setCurrentRow(model->getTextures().size()-1);
    }

    if(items.size() == 1){
        ui->pbDownModel->setEnabled(false);
        ui->pbUpModel->setEnabled(false);
    }
}

//On click texture
void DialogModel::itemActivated(QListWidgetItem* i){
    UNUSED(i);
    ui->cbTextureType->setEnabled(true);
    ui->stackedWidget->setEnabled(true);
    ui->pbSaveZone->setEnabled(true);
    ui->widgetSelect->getLabel()->setVisible(false);
    ui->widgetSelect->getTextEdit()->setVisible(false);
    ui->widgetSelect->getVideo()->setVisible(false);


    int pos = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
    if (model->getTextures().value(pos)->getType() == Texture::TEXT){
        Texture* t = model->getTextures().value(pos);
        TextureTXT* test = (TextureTXT*) t;
        QString textTexture = test->getData();
        qDebug() << "Texte texture = " << textTexture << endl;
        ui->teText->setText(textTexture);
        firstload = 1;
        ui->stackedWidget->setCurrentIndex(0);
        ui->cbTextureType->setCurrentIndex(0);

        ui->widgetSelect->getTextEdit()->setText(textTexture);
        ui->widgetSelect->getTextEdit()->setVisible(true);
    } else if(model->getTextures().value(pos)->getType() == Texture::IMG) {
        Texture* t = model->getTextures().value(pos);
        TextureIMG* test = (TextureIMG*) t;
        QString filename = test->getLocalPath();
        qDebug() << "FILENAME texture = " << filename << endl;
        QImageReader *reader = new QImageReader();
        reader->setFileName(filename);
        QImage image =reader->read();
        QPixmap map=QPixmap::fromImage(image);

        ui->lePathIMG->setText(filename);

        ui->stackedWidget->setCurrentIndex(1);
        ui->cbTextureType->setCurrentIndex(1);
        ui->widgetSelect->getLabel()->setPixmap(map);
        ui->widgetSelect->getLabel()->setVisible(true);
        ui->gbModele->setEnabled(true);
    } else if (model->getTextures().value(pos)->getType() == Texture::MOV) {
        Texture* t = model->getTextures().value(pos);
        TextureMOV* tmov = (TextureMOV*) t;
        ui->stackedWidget->setCurrentIndex(2);
        ui->cbTextureType->setCurrentIndex(2);
        ui->lePathMOV->setText(tmov->getLocalPath());
        ui->gbModele->setEnabled(true);

        QMediaPlaylist *playlist = new QMediaPlaylist(this);
        playlist->addMedia(QUrl::fromLocalFile(tmov->getLocalPath()));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        QMediaPlayer *player = new QMediaPlayer(this);
        player->setPlaylist(playlist);

        player->setVideoOutput(ui->widgetSelect->getVideo());
        ui->widgetSelect->getVideo()->show();
        player->play();


        ui->widgetSelect->getVideo()->setVisible(true);
    }

}

void DialogModel::buttonPlus1(){
    ui->pbRemoveZone->setEnabled(true);
    QString name = "Nouveau";
    Model *m = new Model(name,canva->getItems().size());
    canva->addModel(m);
    ui->ModelList->clear();

    QVector<Model*> items = canva->getItems();
    for(int i = 1; i < items.size()+1; i++) {
        ui->ModelList->addItem("Zone " + QString::number(i));
    }

    if(items.size() == 1){
        ui->pbDownModel->setEnabled(false);
        ui->pbUpModel->setEnabled(false);
    }else{
        ui->pbDownModel->setEnabled(true);
        ui->pbUpModel->setEnabled(true);
    }
    ui->pbAddZone->setEnabled(false);
    ui->buttonBox->setEnabled(false);

}

void DialogModel::buttonMoins1(){

    if(ui->ModelList->selectedItems().size()<1)
        return;

    int pos_to_suppress = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();
    canva->getItems().remove(pos_to_suppress);
    ui->ModelList->clear();

    QVector<Model*> items = canva->getItems();
    for(int i = 1; i < items.size()+1; i++) {
        ui->ModelList->addItem("Zone " + QString::number(i));
    }

    if(items.size()<=1) {
        ui->cbTextureType->setEnabled(false);
        ui->stackedWidget->setEnabled(false);
        ui->gbText->setEnabled(false);
        ui->pbSaveZone->setEnabled(false);
        ui->gbModele->setEnabled(true);
        ui->buttonBox->setEnabled(true);
        ui->pbRemoveZone->setEnabled(false);
        ui->pbAddZone->setEnabled(true);
        return;
    } else {
        ui->TextureList->setCurrentRow(items.size()-1);
    }

}

//On click model
void DialogModel::itemActivated1(QListWidgetItem* i){
    UNUSED(i);
    ui->cbTextureType->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    ui->pbSaveZone->setEnabled(false);
    ui->widgetSelect->getLabel()->setVisible(false);
    ui->widgetSelect->getTextEdit()->setVisible(false);
    ui->widgetSelect->getVideo()->setVisible(false);
    ui->gbText->setEnabled(true);




    if(ui->ModelList->selectedItems().size()<1) return;
    int pos = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();
    model = canva->getItems().value(pos);

    ui->TextureList->clear();

    for(int i = 1; i < model->getTextures().size()+1; i++){
        ui->TextureList->addItem("Page " + QString::number(i));
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
        ui->widgetSelect->getVideo()->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        ui->widgetSelect->getTextEdit()->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        ui->widgetSelect->getRubberBand()->show();
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }

}

void DialogModel::changetext(){
    QString text = ui->teText->toPlainText();

    if(firstload){
        firstload=0;
    } else {
        TextureTXT * ttext = new TextureTXT(text);
        int pos_to_suppress = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();
        model->getTextures().remove(pos_to_suppress);
        model->getTextures().insert(pos_to_suppress,ttext);
        model->setModified(true);
    }


    ui->widgetSelect->getLabel()->setVisible(false);
    ui->widgetSelect->getTextEdit()->setVisible(false);
    ui->widgetSelect->getTextEdit()->setText(text);
    ui->widgetSelect->getTextEdit()->setVisible(true);

    QFont font = ui->widgetSelect->getTextEdit()->font();

    QRect cRect = ui->widgetSelect->getTextEdit()->contentsRect();
    QRect lRect = ui->widgetSelect->getLabel()->contentsRect();

    if( ui->widgetSelect->getTextEdit()->toPlainText().isEmpty() )
        return;

    int flags = Qt::TextWordWrap; //more flags if needed

    int fontSize = 1;
    qDebug() << "content size:" << cRect.width() << "," << cRect.height() << endl;
    qDebug() << "label size:" << lRect.width() << "," << lRect.height() << endl;
    while( true )
    {
        QFont f(font);
        f.setPixelSize( fontSize );
        QRect r = QFontMetrics(f).boundingRect(cRect,flags, ui->widgetSelect->getTextEdit()->toPlainText() );
        if (r.height() <= cRect.height() )
            fontSize++;
        else
            break;
    }

    font.setPixelSize(fontSize);
    ui->widgetSelect->getTextEdit()->setFont(font);


}

void DialogModel::modelEnregistrement(){

    WidgetSelection *widget = ui->widgetSelect;
    QRect rect = widget->getRectSelection();
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
        ui->pbAddZone->setEnabled(true);

    } else {
        qDebug() << "Selection Nulle" << endl;

    }
}

void DialogModel::on_pbUpModel_clicked()
{
    if(ui->ModelList->selectedItems().size()<1)
        return;

    int pos = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();

    if(pos == 0)
        return;

    model = canva->getItems().value(pos);


    canva->getItems().remove(pos);
    canva->getItems().insert(pos-1,model);
    ui->ModelList->clear();

    QVector<Model*> items = canva->getItems();
    for(int i = 1; i < items.size()+1; i++) {
        ui->ModelList->addItem("Zone " + QString::number(i));
    }
}

void DialogModel::on_pbDownModel_clicked()
{
    if(ui->ModelList->selectedItems().size()<1)
        return;

    int pos = ui->ModelList->selectionModel()->selectedIndexes().at(0).row();

    if(pos == canva->getItems().size()-1)
        return;

    model = canva->getItems().value(pos);

    canva->getItems().remove(pos);
    canva->getItems().insert(pos+1,model);
    ui->ModelList->clear();

    QVector<Model*> items = canva->getItems();
    for(int i = 1; i < items.size()+1; i++) {
        ui->ModelList->addItem("Zone " + QString::number(i));
    }
}


void DialogModel::on_pbUpTexture_clicked()
{
    if(ui->TextureList->selectedItems().size()<1)
        return;

    int pos = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();

    if(pos == 0)
        return;

    t= model->getTextures().value(pos);

    model->getTextures().remove(pos);
    model->getTextures().insert(pos-1,t);

    ui->TextureList->clear();

    QVector<Texture*> items = model->getTextures();
    for(int i = 1; i < items.size()+1; i++) {
        ui->TextureList->addItem("Page " + QString::number(i));
    }

}

void DialogModel::on_pbDownTexture_clicked()
{
    if(ui->TextureList->selectedItems().size()<1)
        return;

    int pos = ui->TextureList->selectionModel()->selectedIndexes().at(0).row();

    if(pos == model->getTextures().size()-1)
        return;

    t= model->getTextures().value(pos);

    model->getTextures().remove(pos);
    model->getTextures().insert(pos+1,t);

    ui->TextureList->clear();

    QVector<Texture*> items = model->getTextures();
    for(int i = 1; i < items.size()+1; i++) {
        ui->TextureList->addItem("Page " + QString::number(i));
    }
}
