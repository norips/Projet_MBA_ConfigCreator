#include "model.h"
#include "Config/modelitem.h"


int Model::s_id = 0;
Model::Model(QString& name,int id)
{
    this->name = name;
    this->id = id;
}

modelItem* Model::toItem(){
    modelItem* m = new modelItem(name,this);
    return m;
}

void Model::addTexture(QPixmap *pix){
    textures.clear();
    textures.append(pix);
}
