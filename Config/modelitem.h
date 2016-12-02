#ifndef MODELITEM_H
#define MODELITEM_H

#include <QListWidgetItem>
#include <QVector>
#include "texture.h"
class modelItem : public QListWidgetItem
{
public:
    modelItem(const QString& text);
    QString getName();
private:
    QVector<texture *> textures;
    QString name;
    QString type;
    QString tlc;
    QString trc;
    QString blc;
    QString brc;

};

#endif // MODELITEM_H
