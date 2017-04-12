#ifndef PREVIEW_H
#define PREVIEW_H

#include <QDialog>
#include <QVector>
#include <QLabel>
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
    QLabel* create_pixmap(Model * m, int pos);
    void plus_texture();
    void moins_texture();
    void on_pushButton_clicked();

private:
    Ui::Preview *ui;
    Canva *canva;
    Texture *t;
    QVector<QLabel *> arrLab;

    bool first;
    double ratioX;
    double ratioY;
    int i;
};

#endif // PREVIEW_H
