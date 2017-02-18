#include "canva.h"
#include "filedownloader.h"

int Canva::s_id = 0;
Canva::Canva(const QPixmap &img,const QString &text,const QString &path)
{
    this->img = img;
    this->name = text;
    pathToFile = path;
    this->id = Canva::s_id++;
}

void Canva::addModel(Model *m){
    items.append(m);
}

QVector<Model*> Canva::getItems(){
    return items;
}


void Canva::setPix(const QPixmap& pix){
    img = pix;
}

QPixmap Canva::getPix() const{
    return img;
}

canvaItem* Canva::toItem(){
    canvaItem* c = new canvaItem(QIcon(img),name,id);
    return c;
}

QString Canva::getName(){
    return name;
}
void Canva::setModified(bool val) {
    mod = val;
}

bool Canva::modified(){
    return mod;
}
