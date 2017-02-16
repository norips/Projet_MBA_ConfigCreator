#ifndef TEXTUREIMG_H
#define TEXTUREIMG_H
#include <QPixmap>
#include "texture.h"

class TextureIMG : public Texture
{
public:
    TextureIMG(QPixmap& pix);
    int getType();
    QPixmap getData();
private:
    QPixmap pix;
};

#endif // TEXTUREIMG_H
