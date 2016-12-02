#ifndef MODELITEM_H
#define MODELITEM_H

#include <QListWidgetItem>
#include <QVector>
#include <Config/model.h>
class Model;
class modelItem : public QListWidgetItem
{
public:
    modelItem(const QString& text,Model* m);
    QString getName();
    int getCanvaID();
    Model* getModel();
private:
    int canvaID;
    int modID;
    QString name;
    Model* model;

};

#endif // MODELITEM_H
