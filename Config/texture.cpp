#include "texture.h"

Texture::Texture()
{

}
void Texture::setModified(bool val) {
    mod = val;
}

bool Texture::modified(){
    return mod;
}
