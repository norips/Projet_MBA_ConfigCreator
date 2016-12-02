#include "canvaitem.h"
#include <QUrl>
canvaItem::canvaItem(const QIcon &img,const QString &text,int id) : QListWidgetItem(img,text)
{
    corID = id;
    this->text = text;
}

QString canvaItem::getText(){
    return text;
}

QString canvaItem::getPath(){
    return pathToFile;
}

int canvaItem::getID(){
    return corID;
}
