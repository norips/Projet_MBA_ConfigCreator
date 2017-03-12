#include "model.h"
#include "Config/modelitem.h"


int Model::s_id = 0;
Model::Model(QString& name,int id)
{
    this->name = name;
    this->id = id;
    this->tlc = "";
    this->blc = "";
    this->trc = "";
    this->brc = "";
}

modelItem* Model::toItem(){
    modelItem* m = new modelItem(name,this);
    return m;
}

void Model::addTexture(Texture *tex){
    textures.append(tex);
}

QVector<Texture *> Model::getTextures() {
    return textures;
}
void Model::setModified(bool val) {
    mod = val;
}

bool Model::modified(){
    return mod;
}
