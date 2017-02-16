#ifndef TEXTURETXT_H
#define TEXTURETXT_H

#include "texture.h"
#include <QString>
class TextureTXT : public Texture
{
public:
    TextureTXT(QString texte);
    int getType();
    QString getData();
private:
    QString texte;

};

#endif // TEXTURETXT_H
