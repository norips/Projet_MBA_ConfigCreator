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
    QString getLocalPath();
    void setFile(File f);
    File& getFile();
    void setLocalPath(QString path);
private:
    QPixmap pix;
    File f;
    QString localPath;
};

#endif // TEXTUREIMG_H
