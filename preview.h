#ifndef PREVIEW_H
#define PREVIEW_H

#include <QDialog>
#include "Config/canva.h"
#include "Config/model.h"
#include "Config/texture.h"

namespace Ui {
class Preview;
}

class Preview : public QDialog
{
    Q_OBJECT

public:
    explicit Preview(QWidget *parent = 0, Canva *c = NULL);
    ~Preview();

private slots:
    void create_pixmap(Model * m);

private:
    Ui::Preview *ui;
    Canva *canva;
    Texture *t;

    double ratioX;
    double ratioY;
};

#endif // PREVIEW_H
