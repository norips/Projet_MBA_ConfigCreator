#ifndef CANVA_H
#define CANVA_H
#include <QVector>
#include <QObject>
#include "model.h"
#include "canvaitem.h"
#include "feature.h"
#include "modified.h"

class Canva: public Modified
{
public:
    Canva(const QPixmap &img,const QString &name,const QString &path);
    Canva();
    QString getName();
    void setName(QString &name);
    void addModel(Model* m);
    QVector<Model*> getItems();
    void setPix(const QPixmap& pix);
    QPixmap getPix() const;
    canvaItem* toItem();
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
};

#endif // CANVA_H
