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
    void addTexture(Texture*);
    QVector<Texture *>& getTextures();
    QString name;
    QString type;
    QString tlc;
    QString trc;
    QString blc;
    QString brc;
    bool modified();
    void setModified(bool);

private:
    static int s_id;
    int id;
    QVector<Texture *> textures;
    bool mod;
};

#endif // MODEL_H
