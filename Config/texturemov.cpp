#include "texturemov.h"

TextureMOV::TextureMOV()
{

}
int TextureMOV::getType() {
    return Texture::MOV;
}

QString TextureMOV::getLocalPath() {
    return this->localPath;
}

void TextureMOV::setLocalPath(QString path) {
    this->localPath = path;
}

void TextureMOV::setFile(File f) {
    this->f = f;
}

File& TextureMOV::getFile() {
    return f;
}
