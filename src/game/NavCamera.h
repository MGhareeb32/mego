#ifndef NAVCAMERA_H_
#define NAVCAMERA_H_

#include "Internal.h"

namespace game {

class NavCamera : public Camera {
    GLfloat speed_;
public:
    NavCamera(GLfloat speed) : speed_(speed) {
        persp(50, screen_size().x / screen_size().y);
        lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 1));
    }

    virtual ~NavCamera() {
    }

    void update() {
        GLfloat speed = 2 - game::key_down_[' '] * 1.5f;

        // camera
        if (game::key_down_['a'])
         translate(-speed * u() * speed_);
        if (game::key_down_['d'])
         translate(+speed * u() * speed_);
        if (game::key_down_['r'])
         translate(-speed * v() * speed_);
        if (game::key_down_['f'])
         translate(+speed * v() * speed_);
        if (game::key_down_['w'])
         translate(-speed * n() * speed_);
        if (game::key_down_['s'])
         translate(+speed * n() * speed_);

        // fps controls
        glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
        transform(fpsRotation(speed * delta, GL_FALSE, GL_TRUE));
    }
};

}

#endif
