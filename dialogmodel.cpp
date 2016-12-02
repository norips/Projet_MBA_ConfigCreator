#include "dialogmodel.h"
#include "ui_dialogmodel.h"

DialogModel::DialogModel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModel)
{
    ui->setupUi(this);
}

DialogModel::~DialogModel()
{
    delete ui;
}

void DialogModel::on_buttonBox_accepted()
{

}
