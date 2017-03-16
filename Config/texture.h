#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include "modified.h"

class Texture: public Modified
{
public:
    enum XEnumType {
        TEXT = 0,
        IMG = 1
    };
    Texture();
    virtual int getType() = 0;
};

#endif // TEXTURE_H
