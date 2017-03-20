#include "textureimg.h"

TextureIMG::TextureIMG(QPixmap &pix)
{
    this->pix = pix;
}

TextureIMG::TextureIMG(QPixmap &pix,QString &url, QString &md5)
{
    this->pix = pix;
    this->f = File("",url,md5);
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
