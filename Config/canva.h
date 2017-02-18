#ifndef CANVA_H
#define CANVA_H
#include <QVector>
#include <QObject>
#include "model.h"
#include "canvaitem.h"
#include "feature.h"

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
    bool modified();
    void setModified(bool);
    Feature* getFeature();
    void setFeature(Feature *f);

private:
    static int s_id;
    int id;
    QString name;
    QString pathToFile;
    QPixmap img;
    QVector<Model*> items;
    Feature *feature;
    bool mod;
};

#endif // CANVA_H
