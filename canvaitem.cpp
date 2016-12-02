#include "canvaitem.h"
#include <QUrl>
canvaItem::canvaItem(const QPixmap &img,const QString &text,const QString &path) : QListWidgetItem(QIcon(img),text)
{
    this->img = img;
    this->text = text;
    pathToFile = path;
}

QString canvaItem::getText(){
    return text;
}

QString canvaItem::getPath(){
    return pathToFile;
}

void canvaItem::addModel(modelItem *m){
    items.append(m);
}

QVector<modelItem*> canvaItem::getItems(){
    return items;
}


void canvaItem::setPix(const QPixmap& pix){
    img = pix;
}

QPixmap canvaItem::getPix() const{
    return img;
}
