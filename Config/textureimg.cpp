#include "textureimg.h"

TextureIMG::TextureIMG(QPixmap &pix)
{
    this->pix = pix;
}
int TextureIMG::getType() {
    return Texture::IMG;
}

QPixmap TextureIMG::getData() {
    return pix;
}
