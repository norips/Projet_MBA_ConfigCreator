#include "formtableau.h"
#include "ui_formtableau.h"
#include "Config/modelitem.h"
#include <QFileDialog>
formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QWidget(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    ui->leTitle->setText(item->getText());
    QPixmap pix(item->icon().pixmap(ui->lbPix->rect().size()));
    ui->lbPix->setPixmap(pix);
    QVector<modelItem*> items = item->getItems();
    foreach (modelItem* m, items) {
       ui->lvListeModele->addItem(m);
    }
    canva = item;
}

void formTableau::on_pbPath_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open image"), QDir::currentPath(), tr("Images(*.jpg *.jpeg)"));
    canva->setPix(QPixmap(fileName));
    ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));

}
