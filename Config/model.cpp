#include "model.h"
#include "Config/modelitem.h"
Model::Model(QString& name)
{
    this->name = name;
}

modelItem* Model::toItem(){
    modelItem* m = new modelItem(name);
    return m;
}
