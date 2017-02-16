#include "dialogmodel.h"
#include "ui_dialogmodel.h"
#include "Config/modelitem.h"
#include "Config/configholder.h"
#include "Config/texture.h"
#include "Config/textureimg.h"
#include <QDebug>
#include <QFileDialog>
DialogModel::DialogModel(QWidget *parent,modelItem* m) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);
    model = m->getModel();

    ui->leTlc->setText(model->tlc);
    ui->leTrc->setText(model->trc);
    ui->leBlc->setText(model->blc);
    ui->leBrc->setText(model->brc);
    ui->leName->setText(model->name);
    if(model->getTextures().size() > 0 && (model->getTextures().at(0))->getType() == Texture::IMG) {
        TextureIMG *timg = dynamic_cast<TextureIMG *>(model->getTextures().at(0));
        ui->lbTexture->setPixmap(timg->getData());
    }

}

DialogModel::~DialogModel()
{
    delete ui;
}

void DialogModel::on_buttonBox_accepted()
{
    QString tlc,trc,blc,brc;
    tlc = ui->leTlc->text();
    trc = ui->leTrc->text();
    blc = ui->leBlc->text();
    brc = ui->leBlc->text();
    QRegExp exp("^-?[0-9]+,-?[0-9]+,-?[0-9]+$");
    if(tlc.contains(exp) && trc.contains(exp) && blc.contains(exp) && brc.contains(exp) ) {
        qDebug() << "Valid" << endl;
        model->tlc = tlc;
        model->trc = trc;
        model->blc = blc;
        model->brc = brc;
        model->name = ui->leName->text();
        QPixmap pix = QPixmap::fromImage(QImage(ui->lePath->text()));
        TextureIMG *timg = new TextureIMG(pix);
        model->addTexture(timg);
        this->hide();
    } else {
        qDebug() << "Invalid" << endl;
    }
}

void DialogModel::on_pbPath_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open image"), QDir::currentPath(), tr("Images(*.jpg *.jpeg)"));
    if(fileName != NULL) {
        ui->lePath->setText(fileName);
    }

}
