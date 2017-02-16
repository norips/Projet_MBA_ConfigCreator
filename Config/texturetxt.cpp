#include "texturetxt.h"

TextureTXT::TextureTXT(QString texte)
{
    this->texte = texte;
}

int TextureTXT::getType() {
    return Texture::TEXT;
}

QString TextureTXT::getData() {
    return texte;
}
