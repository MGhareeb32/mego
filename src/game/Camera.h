#ifndef CAMERA_H_
#define CAMERA_H_

#include "OpenGL.h"
#include "Entity.h"
#include "Internal.h"

namespace game {

class Camera : public Entity {
    glm::mat4 proj_;
public:
    Camera();
    virtual ~Camera();

    void lookAt(glm::vec3 eye, glm::vec3 coi, glm::vec3 upv);
    void ortho(GLfloat xleft = -1, GLfloat xright = 1,
               GLfloat ybottom = -1, GLfloat ytop = 1,
               GLfloat znear = 1, GLfloat zfar = 8);
    void persp(GLfloat fov = 40, GLfloat aspect = 1,
               GLfloat n = 1, GLfloat f = 8);

    glm::mat4 getProjectionMatrix() { return proj_; }
    glm::mat4 getViewMatrix() { return transform_i(); }

    glm::vec3 arcballVector(glm::vec2 p, glm::vec3 off);
    glm::mat4 arcballRotation(glm::vec2 p1, glm::vec2 p2, glm::vec3 off);
};

}

#endif
