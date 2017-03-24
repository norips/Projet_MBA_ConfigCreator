#include "formtableau.h"
#include "ui_formtableau.h"
#include "Config/model.h"
#include <QFileDialog>
#include "Config/configholder.h"
#include "dialogmodel.h"
#include <QWidget>
#include <QList>
formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QDialog(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    canva = ConfigHolder::Instance()->getCanvas().at(item->getID());
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
        canva->setModified(true);
        ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));
    }
}


void formTableau::on_pbMod_released()
{
    if(ui->lvListeModele->selectedItems().size()<1) return;
    modelItem *m = (modelItem*) ui->lvListeModele->selectedItems().at(0);
    DialogModel* dialog = new DialogModel(this,m,canva);
    dialog->exec();
}

void formTableau::on_pbAdd_clicked()
{
    QString name = "Nouveau";
    Model *m = new Model(name,canva->getItems().size());
    canva->addModel(m);
    modelItem* mi = m->toItem();
    ui->lvListeModele->addItem(mi);
    DialogModel* dialog = new DialogModel(this,mi,canva);
    dialog->show();
}

void formTableau::on_pushButton_released()
{
    QString name_tableau = ui->leTitle->text();
    canva->setName(name_tableau);

    this->close();

}

void formTableau::on_pbDel_released()
{
    if(ui->lvListeModele->selectedItems().size()<1) return;

    QModelIndexList indexes = ui->lvListeModele->selectionModel()->selectedIndexes();
    qDebug() << ui->lvListeModele->selectionModel()->selectedIndexes().at(0).row() << endl;

    int pos_to_suppress = ui->lvListeModele->selectionModel()->selectedIndexes().at(0).row();
    canva->getItems().remove(pos_to_suppress);
    ui->lvListeModele->clear();

    QVector<Model*> items = canva->getItems();
    foreach (Model* m, items) {
       ui->lvListeModele->addItem(m->toItem());

    }

}
