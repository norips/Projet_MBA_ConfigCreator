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
    bool modified();
    void setModified(bool);
private:
    bool mod;
};

#endif // TEXTURE_H
