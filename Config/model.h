#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QVector>
#include "texture.h"
#include "Config/modelitem.h"
class Model
{
public:
    Model(QString& name);
    modelItem* toItem();

private:
    QVector<texture *> textures;
    QString name;
    QString type;
    QString tlc;
    QString trc;
    QString blc;
    QString brc;
};

#endif // MODEL_H
