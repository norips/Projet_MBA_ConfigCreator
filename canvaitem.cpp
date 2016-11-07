#include "canvaitem.h"

canvaItem::canvaItem(const QIcon &icon,const QString &text,const QString &path) : QListWidgetItem(icon,text)
{
    this->text = text;
    pathToFile = path;
}

QString canvaItem::getText(){
    return text;
}

QString canvaItem::getPath(){
    return pathToFile;
}
