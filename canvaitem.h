#ifndef CANVAITEM_H
#define CANVAITEM_H

#include <QIcon>
#include <QListWidgetItem>
#include <QString>
#include <QObject>
#include <QPixmap>
#include "filedownloader.h"
#include "Config/modelitem.h"
class canvaItem : public QListWidgetItem, public QObject
{
public:
    canvaItem(const QIcon &img,const QString &text,int parentID);
    QString getText();
    QString getPath();

    int getParentID();

    void addModel(modelItem* m);
    QVector<modelItem*> getItems();
    void setPix(const QPixmap& pix);
    QPixmap getPix() const;
private:
    int parID;
    QString text;
    QString pathToFile;
    QPixmap img;
    QVector<modelItem*> items;
};

#endif // CANVAITEM_H
