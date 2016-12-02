#include "formtableau.h"
#include "ui_formtableau.h"
#include "Config/model.h"
#include <QFileDialog>
#include "Config/configholder.h"
#include "dialogmodel.h"
formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QWidget(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    canva = ConfigHolder::Instance().getCanvas().at(item->getID());
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


void formTableau::on_pbMod_released()
{
    if(ui->lvListeModele->selectedItems().size()<1) return;
    modelItem *m = (modelItem*) ui->lvListeModele->selectedItems().at(0);
    DialogModel* dialog = new DialogModel(0,m);
    dialog->show();
}
