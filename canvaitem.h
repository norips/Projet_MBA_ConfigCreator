#ifndef CANVAITEM_H
#define CANVAITEM_H

#include <QIcon>
#include <QListWidgetItem>
#include <QString>
#include <QObject>
#include <QPixmap>
#include "filedownloader.h"
class canvaItem : public QListWidgetItem, public QObject
{
public:
    canvaItem(const QIcon &img,const QString &text,int id);
    QString getText();
    QString getPath();

    int getID();
private:
    int corID;
    QString text;
    QString pathToFile;
};

#endif // CANVAITEM_H
