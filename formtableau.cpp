#include "formtableau.h"
#include "ui_formtableau.h"
formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QWidget(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    ui->leTitle->setText(item->getText());
    QPixmap pix(item->getPath());
    pix = pix.scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio);
    ui->lbPix->setPixmap(pix);
    ui->lvListeModele->addItem("modele1");
    ui->lvListeModele->addItem("modele2");
    ui->lvListeModele->addItem("modele3");
}
