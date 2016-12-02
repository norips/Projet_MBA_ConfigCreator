#include "formtableau.h"
#include "ui_formtableau.h"
#include "Config/model.h"
#include <QFileDialog>
#include "Config/configholder.h"
formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QWidget(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    canva = ConfigHolder::Instance().getCanvas().at(item->getParentID());
    ui->leTitle->setText(canva->getName());
    ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));
    QVector<Model*> items = canva->getItems();
    foreach (Model* m, items) {
       ui->lvListeModele->addItem(m->toItem());
    }
}

void formTableau::on_pbPath_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open image"), QDir::currentPath(), tr("Images(*.jpg *.jpeg)"));
    if(fileName != NULL) {
        canva->setPix(QPixmap(fileName));
        ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));
    }
}

