#include "preview.h"
#include "ui_preview.h"
#include <QPixmap>
#include <QRect>
#include <Config/textureimg.h>
#include <Config/texturetxt.h>
#include <textedit.h>
#include <QImageReader>
#include <QFont>
#include <QAbstractButton>
#include <QPushButton>

Preview::Preview(QWidget *parent, Canva *c) :
    QDialog(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    connect(ui->page_plus, SIGNAL(pressed()), this, SLOT(plus_texture()));
    connect(ui->page_moins, SIGNAL(pressed()), this, SLOT(moins_texture()));

    canva = c;
    i=0;
    first = true;

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
    for(int j = 0; j < items.size();j++) {
       create_pixmap(canva->getItems().value(j), i);
    }

}

Preview::~Preview()
{
    delete ui;
}

void Preview::create_pixmap(Model *model, int position)
{
    int pos =0;
    if (position >=0){
        pos = position % model->getTextures().size();
    }else{
        while(position<0){
            position = position + model->getTextures().size();
            pos = position;
        }
    }
    TextEdit * text = new TextEdit("",this);

    QLabel * label = new QLabel(this);
    label->setScaledContents(true);
    label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setWordWrap(true);

    if( !model->tlc.isEmpty() && !model->trc.isEmpty() && !model->blc.isEmpty() && !model->brc.isEmpty()) {
        QStringList lTLC = model->tlc.split(",");
        double tlcX = lTLC.at(0).toDouble() / ratioX;
        double tlcY = lTLC.at(1).toDouble() / ratioY;
        tlcY = -tlcY;

        QStringList lTRC = model->trc.split(",");
        double trcY = lTRC.at(1).toDouble() / ratioY;
        trcY = -trcY;

        QStringList lBLC = model->blc.split(",");
        double blcY = lBLC.at(1).toDouble() / ratioY;
        blcY = -blcY;

        QStringList lBRC = model->brc.split(",");
        double brcX = lBRC.at(0).toDouble() / ratioX;
        double brcY = lBRC.at(1).toDouble() / ratioY;
        brcY = -brcY;

        tlcX += (ui->tableau->x()) ;
        tlcY += (ui->tableau->y() + ui->tableau->height()) ;
        brcX += (ui->tableau->x()) ;
        brcY += (ui->tableau->y() + ui->tableau->height());

        label->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
        text->setGeometry(QRect(QPoint(tlcX,tlcY),QPoint(brcX,brcY)));
    }

    if (model->getTextures().value(pos)->getType() == Texture::TEXT){
        text->raise();
        Texture* t = model->getTextures().value(pos);
        TextureTXT* test = (TextureTXT*) t;
        QString textTexture = test->getData();
        qDebug() << "Texte texture = " << textTexture << endl;
        text->setText(textTexture);


        QFont font = text->font();

        QRect cRect = text->contentsRect();

        if(text->toPlainText().isEmpty() )
                return;

        int flags = Qt::TextWordWrap; //more flags if needed

             int fontSize = 1;
              while( true )
              {
                          QFont f(font);
                               f.setPixelSize( fontSize );
                          QRect r = QFontMetrics(f).boundingRect(cRect,flags, text->toPlainText() );
                          if (r.height() <= cRect.height() )
                                fontSize++;
                          else
                                break;
              }

             font.setPixelSize(fontSize);
             text->setFont(font);


        label->setVisible(false);
        text->setVisible(true);

    } else if(model->getTextures().value(pos)->getType() == Texture::IMG) {
        label->raise();
        Texture* t = model->getTextures().value(pos);
        TextureIMG* test = (TextureIMG*) t;
        QString filename = test->getLocalPath();

        QImageReader *reader = new QImageReader();
        reader->setFileName(filename);
        QImage image =reader->read();
        QPixmap map=QPixmap::fromImage(image);

        label->setPixmap(map);
        label->setVisible(true);
        text->setVisible(false);
    }
}

void Preview::plus_texture()
{
    QVector<Model*> items = canva->getItems();
    i=i+1;
    for(int j = 0; j < items.size();j++) {
       create_pixmap(canva->getItems().value(j), i);
    }

}

void Preview::moins_texture()
{
    QVector<Model*> items = canva->getItems();
    i=i-1;
    for(int j = 0; j < items.size();j++) {
       create_pixmap(canva->getItems().value(j), i);
    }

}
