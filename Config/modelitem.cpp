#include "modelitem.h"

modelItem::modelItem(const QString& text) : QListWidgetItem(text)
{
    this->name = text;
}
QString modelItem::getName(){
    return name;
}
