#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include "modified.h"

class Texture: public Modified
{
public:
    enum XEnumType {
        TEXT = 0,
        IMG = 1,
        MOV = 2
    };
    Texture();
    virtual int getType() = 0;
private:
};

#endif // TEXTURE_H
