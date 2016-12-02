#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QVector>
#include <QPixmap>
#include "texture.h"
class modelItem;
class Model
{
public:
    Model(QString& name,int id);
    modelItem* toItem();
    void addTexture(QPixmap*);
    QString name;
    QString type;
    QString tlc;
    QString trc;
    QString blc;
    QString brc;

private:
    static int s_id;
    int id;
    QVector<QPixmap *> textures;
};

#endif // MODEL_H
