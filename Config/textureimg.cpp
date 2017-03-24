#include "textureimg.h"

TextureIMG::TextureIMG(QPixmap &pix)
{
    this->pix = pix;
}

TextureIMG::TextureIMG(QPixmap &pix,QString &url, QString &md5)
{
    this->pix = pix;
    this->f = File("",url,md5);
    this->localPath = "";
}
int TextureIMG::getType() {
    return Texture::IMG;
}

QPixmap TextureIMG::getData() {
    return pix;
}

QString TextureIMG::getUrl() {
    return f.getPath();
}

QString TextureIMG::getMD5() {
    return f.getMD5();
}

QString TextureIMG::getLocalPath() {
    return this->localPath;
}

void TextureIMG::setLocalPath(QString path) {
    this->localPath = path;
}
