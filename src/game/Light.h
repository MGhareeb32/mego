#ifndef LIGHT_H_
#define LIGHT_H_

#include "Entity.h"

namespace game {

class Light : public Entity {
    glm::mat3 color_;
public:

    Light(glm::vec3 color = glm::vec3(1.f, 1.f, 1.f)) : Entity(),
        color_(glm::mat3(color, color, color)) {};

    Light(glm::mat3 color = glm::mat3(1)) : Entity(), color_(color) {};
    ~Light() {};

    glm::mat3 color() { return color_; };
    void set_color(glm::mat3 color) { color_ = color; };
    glm::vec3 ambient() { return color_[0]; };
    void set_ambient(glm::vec3 ambient) { color_[0] = ambient; };
    glm::vec3 diffuse() { return color_[1]; };
    void set_diffuse(glm::vec3 diffuse) { color_[1] = diffuse; };
    glm::vec3 specular() { return color_[2]; };
    void set_specular(glm::vec3 specular) { color_[2] = specular; };
};

}

#endif
