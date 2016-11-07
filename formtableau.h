#ifndef FORMTABLEAU_H
#define FORMTABLEAU_H

#include <QWidget>
#include "canvaitem.h"
namespace Ui {
    class formTableau;
}
class formTableau : public QWidget
{
    Q_OBJECT
public:
    explicit formTableau(QWidget *parent = 0,canvaItem *tab=NULL);
private:
    Ui::formTableau *ui;
signals:

public slots:
};

#endif // FORMTABLEAU_H
