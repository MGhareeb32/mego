#include "Camera.h"

using namespace std;
namespace game {

Camera::Camera() : Entity() {
    persp();
    lookAt(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
}

Camera::~Camera() {
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 coi, glm::vec3 upv) {
    resetTransform();

    glm::vec3 newN = glm::normalize(eye - coi);
    glm::vec3 newU = glm::normalize(glm::cross(upv, newN));
    glm::vec3 newV = glm::cross(newN, newU);

    GLfloat angU = glm::degrees(acos(glm::dot(newU, u())));
    glm::vec3 axisU = glm::cross(newU, u());
    rotate(-angU, axisU);

    GLfloat angV = glm::degrees(acos(glm::dot(newV, v())));
    glm::vec3 axisV = glm::cross(newV, v());
    rotate(-angV, axisV);

    translate(eye);
}

void Camera::ortho(GLfloat xleft, GLfloat xright,
                   GLfloat ybottom, GLfloat ytop,
                   GLfloat znear, GLfloat zfar) {
    glm::vec3 near_ = glm::vec3(xleft, ybottom, znear);
    glm::vec3 far_ = glm::vec3(xright, ytop, zfar);
    glm::vec3 add = far_ + near_;
    glm::vec3 sub = far_ - near_;
    proj_ = glm::mat4(1);
    proj_[0][0] = 2 / sub.x;
    proj_[1][1] = 2 / sub.y;
    proj_[2][2] = -2 / sub.z;
    proj_[3][0] = -add.x / sub.x;
    proj_[3][1] = -add.y / sub.y;
    proj_[3][2] = -add.z / sub.z;
}

void Camera::persp(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f) {
    proj_ = glm::mat4(0);
    GLfloat tanHalfFovy = glm::tan(glm::radians(fov) / 2);
    proj_[0][0] = 1 / (aspect * tanHalfFovy);
    proj_[1][1] = 1 / (tanHalfFovy);
    proj_[2][2] = - (f + n) / (f - n);
    proj_[2][3] = - 1;
    proj_[3][2] = - (2 * f * n) / (f - n);
}

glm::vec3 Camera::arcballVector(glm::vec2 p, glm::vec3 off) {
    glm::vec4 out = glm::vec4(glm::clamp(p.x, -1.f, 1.f),
                              glm::clamp(p.y, -1.f, 1.f), 0, 0);
    GLfloat p_squared = glm::dot(p, p);
    if (p_squared <= 1)
        out.z = glm::sqrt(1 - p_squared) + o().z;
    else
        out = glm::normalize(out);
    return glm::vec3(transform() * out);
}

glm::mat4 Camera::arcballRotation(glm::vec2 p1, glm::vec2 p2, glm::vec3 off) {
    glm::vec3 r1 = glm::normalize(arcballVector(p1, off));
    glm::vec3 r2 = glm::normalize(arcballVector(p2, off));
    GLfloat angle = glm::degrees(glm::acos(std::min(1.f, glm::dot(r1, r2))));

    if (glm::equal(r1, r2)[0]
        && glm::equal(r1, r2)[1]
        && glm::equal(r1, r2)[0])
        return glm::mat4(1);
    glm::vec3 axis = glm::cross(r1, r2);

    rotate(angle, -axis, off);
    glm::mat4 trans = transform();
    rotate(angle, axis, off);
    return trans * transform_i();
}

glm::mat4 Camera::fpsRotation(glm::vec2 delta, GLboolean flipY) {
    glm::vec2 d = glm::degrees(game::mouse_pos_ - game::mouse_pos_prev_);
    if (!flipY)
        d.x *= -1.f;
    glm::vec2 zero = glm::vec2(0, 0);
    if (glm::equal(delta, zero)[0] && glm::equal(delta, zero)[1])
        return glm::mat4(1);
    rotate(d.x, glm::vec3(0, 0, 1), o());
    rotate(d.y, u(), o());
    glm::mat4 trans = transform();
    // make sure not to flip
    glm::vec3 oldN_xy = glm::vec3(n().x, n().y, 0);
    rotate(-d.y, u(), o());
    glm::vec3 newN_xy = glm::vec3(n().x, n().y, 0);
    GLfloat cosAngle = glm::dot(newN_xy, oldN_xy);
    rotate(-d.x, glm::vec3(0, 0, 1), o());
    if (cosAngle < 0)
        return glm::mat4(1);
    return trans * transform_i();
}

}
