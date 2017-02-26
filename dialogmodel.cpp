#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include "Config/modelitem.h"
#include "Config/configholder.h"
#include "Config/texture.h"
#include "Config/textureimg.h"
#include "Config/texturetxt.h"
#include <QDebug>
#include <QFileDialog>

DialogModel::DialogModel(QWidget *parent,modelItem* m, Canva *c) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);
    model = m->getModel();

    ui->leName->setText(m->getName());

    canva = c;
    ui->lbpixmap->setPixmap(canva->getPix().scaled(ui->lbpixmap->rect().size(),Qt::KeepAspectRatio));

    ui->zoneSelection->setMouseTracking(true);

    this->widget = new Widget(ui->zoneSelection);
    ui->zoneSelection->setStyleSheet("border: 1px solid black;");
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
    delete ui;
}

void DialogModel::on_buttonBox_accepted()
{
    QRect rect = widget->getRectSelection();
    QString text ;

    int xtlc,ytlc;
    int xtrc, ytrc;
    int xblc, yblc;
    int xbrc, ybrc;

    if(rect.height() !=0 && rect.width() != 0){
        qDebug() << "Valid" << endl;
        text = ui->lbText->text();
        int x =0, y=0, width, height;
        rect.getRect(&x,&y,&width,&height);
        xtlc = x ; ytlc = y;
        xtrc = x+width; ytrc = y;
        xblc= x; yblc = y+height;
        xbrc = x + width; ybrc = y+height;
        model->tlc.append((QString::number(xtlc))).append(",").append(QString::number(ytlc)).append(",0");
        model->trc.append((QString::number(xtrc))).append(",").append(QString::number(ytrc)).append(",0");
        model->blc.append((QString::number(xblc))).append(",").append(QString::number(yblc)).append(",0");
        model->brc.append((QString::number(xbrc))).append(",").append(QString::number(ybrc)).append(",0");
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
