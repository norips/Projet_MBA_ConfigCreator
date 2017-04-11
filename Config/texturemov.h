#ifndef TEXTUREMOV_H
#define TEXTUREMOV_H
#include "texture.h"
#include "file.h"

class TextureMOV
{
public:
    TextureMOV();
    int getType();
    QString getLocalPath();
    void setFile(File f);
    File& getFile();
    void setLocalPath(QString path);
private:

    File f;
    QString localPath;
};

#endif // TEXTUREMOV_H
