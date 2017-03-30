#ifndef FORMTABLEAU_H
#define FORMTABLEAU_H

#include <QDialog>
#include "Config/canva.h"
namespace Ui {
    class formTableau;
}
class formTableau : public QDialog
{
    Q_OBJECT
public:
    explicit formTableau(QWidget *parent = 0,canvaItem *tab=NULL);
private:
    Ui::formTableau *ui;
    Canva *canva;
    canvaItem * i;

signals:

public slots:
private slots:
    void on_pbPath_released();
    void on_pbMod_released();
    void on_pbAdd_clicked();
    void on_pushButton_released();
    void on_pbDel_released();
};

#endif // FORMTABLEAU_H
