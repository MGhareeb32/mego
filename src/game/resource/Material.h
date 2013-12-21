#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Internal.h"
#include "Resource.h"
#include "Texture.h"
#include "TextureLoader.h"

namespace game {

class Material : public Resource {
    glm::vec3 ka_, kd_, ks_;
    GLfloat ns_, tr_;
    Texture *texture_;
public:
    Material(std::string uid,
             glm::vec3 ka = glm::vec3(.2f, .2f, .2f),
             glm::vec3 kd = glm::vec3(.8f, .8f, .8f),
             glm::vec3 ks = glm::vec3(1.f, 1.f, 1.f), GLfloat ns = 0.f,
             GLfloat tr = 1.f, Texture *texture = NULL)
        : Resource(uid),
          ka_(ka), kd_(kd), ks_(ks), ns_(ns), tr_(tr), texture_(texture) {
    };
    ~Material() {};

    glm::vec3 ka() { return ka_; }
    void set_ka(glm::vec3 ka) { ka_ = ka; }
    glm::vec3 kd() { return kd_; }
    void set_kd(glm::vec3 kd) { kd_ = kd; }
    glm::vec3 ks() { return ks_; }
    void set_ks(glm::vec3 ks) { ks_ = ks; }
    GLfloat ns() { return ns_; }
    void set_ns(GLfloat ns) { ns_ = ns; }
    GLfloat tr() { return tr_; }
    void set_tr(GLfloat tr) { tr_ = tr; }
    Texture *texture() { return texture_; }
    void set_texture(Texture *texture) { texture_ = texture; }
};

}

#endif
