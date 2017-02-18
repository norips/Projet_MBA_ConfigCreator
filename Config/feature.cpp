#include "feature.h"

Feature::Feature(QString _name,File _iset, File _fset, File _fset3 ) : iset(_iset), fset(_fset), fset3(_fset3)
{
    name = _name;
}

Feature::Feature(QString _name) : iset(File("null","null","null")), fset(File("null","null","null")), fset3(File("null","null","null"))
{
    name = _name;
}

QString Feature::getName(){
    return name;
}

File Feature::getIset() {
    return iset;
}

File Feature::getFset() {
    return fset;
}
File Feature::getFset3() {
    return fset3;
}

void Feature::setIset(File f) {
    iset = f;
}

void Feature::setFset(File f) {
    fset = f;
}
void Feature::setFset3(File f) {
    fset3 = f;
}


