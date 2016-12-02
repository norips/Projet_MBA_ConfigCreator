#ifndef CANVA_H
#define CANVA_H
#include <QVector>
#include "model.h"
#include "canvaitem.h"
#include <QObject>

class Canva
{
public:
    Canva(const QPixmap &img,const QString &name,const QString &path);
    Canva();
    QString getName();
    void addModel(Model* m);
    QVector<Model*> getItems();
    void setPix(const QPixmap& pix);
    QPixmap getPix() const;
    canvaItem* toItem();
private:
    static int s_id;
    int id;
    QString name;
    QString pathToFile;
    QPixmap img;
    QVector<Model*> items;
};

#endif // CANVA_H
