#include "canva.h"
#include "filedownloader.h"

Canva::Canva(QString& name,QString& url)
{
    this->name = name;
    this->url = url;
}
Canva::Canva() {
    this->name = "null";
    this->url = "null";
}
QString Canva::info() const{
    return name + url;
}
QString Canva::getURL() const{
    return url;
}
QString Canva::getName() const{
    return name;
}
