#include "formtableau.h"
#include "ui_formtableau.h"
#include "Config/model.h"
#include <QFileDialog>
#include "Config/configholder.h"
#include "dialogmodel.h"
#include "preview.h"
#include <QWidget>
#include <QList>
#include <QDialogButtonBox>
#include <QPushButton>

formTableau::formTableau(QWidget *parent, canvaItem *item) :
    QDialog(parent),
    ui(new Ui::formTableau)
{
    ui->setupUi(this);
    i=item;
    canva = ConfigHolder::Instance()->getCanvas().at(item->getID());

    ui->leTitle->setText(canva->getName());
    ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton("Enregistrer",QDialogButtonBox::AcceptRole);
    buttonBox->addButton("Quitter",QDialogButtonBox::RejectRole);

    ui->verticalLayout_4->addWidget(buttonBox);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(on_pushButton_released()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->apercu, SIGNAL(pressed()), this, SLOT(preview_tab()));
}

void formTableau::on_pbPath_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open image"), QDir::currentPath(), tr("Images(*.jpg *.jpeg)"));
    if(fileName != NULL) {
        canva->setPix(QPixmap(fileName));
        canva->setModified(true);
        ui->lbPix->setPixmap(canva->getPix().scaled(ui->lbPix->rect().size(),Qt::KeepAspectRatio));
    }
}



void formTableau::on_pbAdd_clicked()
{
    DialogModel* dialog = new DialogModel(this,i, canva);
    dialog->show();
}

void formTableau::on_pushButton_released()
{
    QString name_tableau = ui->leTitle->text();
    canva->setName(name_tableau);
    this->close();

}

void formTableau::preview_tab()
{
    Preview * preview_tab = new Preview(this, canva);
    preview_tab->show();
}

