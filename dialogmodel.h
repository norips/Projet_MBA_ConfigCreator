#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>
#include <QGroupBox>
#include "Config/model.h"
#include "Config/modelitem.h"
#include "Config/canva.h"
#include "Config/widgetSelection.h"
namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = 0, canvaItem *item = NULL, Canva *c = NULL);
    ~DialogModel();

private slots:
    void on_buttonBox_accepted();
    void openFile();
    void openFile2();
    void buttonPlus();
    void buttonMoins();
    void itemActivated(QListWidgetItem*);
    void buttonPlus1();
    void buttonMoins1();
    void itemActivated1(QListWidgetItem*);
    void changetext();
    void modelEnregistrement();


private:
    Ui::DialogModel *ui;
    Model *model;
    Canva *canva;
    Texture *t;
    modelItem *mo;
    QLabel *test;
    double ratioX;
    double ratioY;
};

#endif // DIALOGMODEL_H
