#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Internal.h"
#include "Resource.h"
#include "libexternal/SOIL.h"

namespace game {

class Texture : public Resource {
    GLuint textureID_;
    int width_, height_;
public:
    Texture(std::string uid, int width, int height, unsigned char* image)
        : Resource(uid), width_(width), height_(height) {

        glGenTextures(1, &textureID_);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    };
    ~Texture() {};

    GLfloat textureID() { return textureID_; }
};

}

#endif
