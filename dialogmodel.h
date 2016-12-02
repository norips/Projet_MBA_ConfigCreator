#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>
#include "Config/model.h"
#include "Config/modelitem.h"
namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = 0,modelItem* m = NULL);
    ~DialogModel();

private slots:
    void on_buttonBox_accepted();

    void on_pbPath_released();

private:
    Ui::DialogModel *ui;
    Model *model;
};

#endif // DIALOGMODEL_H
