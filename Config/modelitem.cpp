#include "modelitem.h"

modelItem::modelItem(const QString& text,Model* m) : QListWidgetItem(text)
{
    this->name = text;
    model = m;
}
QString modelItem::getName(){
    return name;
}

int modelItem::getCanvaID(){
    return canvaID;
}


Model* modelItem::getModel(){
    return model;
}
