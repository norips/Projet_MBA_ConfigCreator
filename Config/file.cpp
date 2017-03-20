#include "file.h"

File::File(QString _name,QString _path,QString _MD5)
{
    name=_name;
    path=_path;
    MD5=_MD5;
}

File::File(){
    name="";
    path="";
    MD5="";
}

QString File::getName() {
    return name;
}

QString File::getPath() {
    return path;
}

QString File::getMD5() {
    return MD5;
}
