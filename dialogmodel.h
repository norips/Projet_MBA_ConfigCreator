#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>
#include <QGroupBox>
#include "Config/model.h"
#include "Config/modelitem.h"
#include "Config/canva.h"
#include "Config/widget.h"
namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = 0,modelItem* m = NULL, Canva *c = NULL);
    ~DialogModel();


private slots:
    void on_buttonBox_accepted();


public:
    Ui::DialogModel *ui;
    Model *model;
    Canva *canva;

    double ratioX;
    double ratioY;
};

#endif // DIALOGMODEL_H
