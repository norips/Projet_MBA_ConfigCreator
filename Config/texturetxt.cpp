#include "texturetxt.h"

TextureTXT::TextureTXT(QString texte) : Texture()
{
    this->texte = texte;
}

int TextureTXT::getType() {
    return Texture::TEXT;
}

QString TextureTXT::getData() {
    return texte;
}
