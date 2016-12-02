#include "canvaitem.h"
#include <QUrl>
canvaItem::canvaItem(const QIcon &img,const QString &text,int parentId) : QListWidgetItem(img,text)
{
    parID = parentId;
    this->text = text;
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

int canvaItem::getParentID(){
    return parID;
}
