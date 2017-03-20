#ifndef TEXTUREIMG_H
#define TEXTUREIMG_H
#include <QPixmap>
#include "texture.h"
#include "file.h"
class TextureIMG : public Texture
{
public:
    TextureIMG(QPixmap& pix);
    TextureIMG(QPixmap& pix,QString& url,QString& md5);
    int getType();
    QString getUrl();
    QString getMD5();
    QPixmap getData();
private:
    QPixmap pix;
    File f;
};

#endif // TEXTUREIMG_H
