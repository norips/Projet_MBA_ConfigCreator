#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
class Texture
{
public:
    enum XEnumType {
        TEXT = 0,
        IMG = 1
    };
    Texture();
    virtual int getType() = 0;
private:
};

#endif // TEXTURE_H
