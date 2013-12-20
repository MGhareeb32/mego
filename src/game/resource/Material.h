#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Internal.h"
#include "Resource.h"
#include "libexternal/SOIL.h"

namespace game {

class Material : public Resource {
    glm::vec3 ka_, kd_, ks_;
    GLfloat ns_, tr_;
    GLuint textureID_;
public:
    Material(std::string uid,
             glm::vec3 ka = glm::vec3(.2f, .2f, .2f),
             glm::vec3 kd = glm::vec3(.8f, .8f, .8f),
             glm::vec3 ks = glm::vec3(1.f, 1.f, 1.f), GLfloat ns = 0.f,
             GLfloat tr = 1.f, std::string textureFile = "")
        : Resource(uid), ka_(ka), kd_(kd), ks_(ks), ns_(ns), tr_(tr) {

        glGenTextures(1, &textureID_);

        int width, height;
        unsigned char* image;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID_);

        image = SOIL_load_image(textureFile.c_str(), &width, &height, 0,
                SOIL_LOAD_RGB);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, image);
        std::cout << height  << " " << width <<  " ... " << std::endl;

        SOIL_free_image_data(image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    };
    ~Material() {};

    glm::vec3 ka() { return ka_; }
    glm::vec3 kd() { return kd_; }
    glm::vec3 ks() { return ks_; }
    GLfloat ns() { return ns_; }
    GLfloat tr() { return tr_; }
    GLfloat textureID() { return textureID_; }
};

}

#endif
