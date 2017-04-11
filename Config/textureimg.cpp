#include "textureimg.h"

TextureIMG::TextureIMG() {
    this->pix = pix;
    this->f = File("","","");
    this->localPath = "";
}

TextureIMG::TextureIMG(QPixmap &pix) : Texture()
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

void TextureIMG::setFile(File f) {
    this->f = f;
}

File& TextureIMG::getFile() {
    return f;
}
