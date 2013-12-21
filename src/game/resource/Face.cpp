#include "Face.h"

namespace game {

Face::Face(int vertexCount, glm::vec3 *vertex, glm::vec2 *uvmap,
        glm::vec3 *normal, GLint mode) {

    nVertex_ = vertexCount;
    drawMode_ = mode;
    vertex_ = std::vector<glm::vec3>(vertex, vertex + nVertex_);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(3, &vbo_[0]);

    int attr_idx = 0;

    // position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 nVertex_ * sizeof(glm::vec3), vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;

    // normal
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 nVertex_ * sizeof(glm::vec3), normal, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;

    // texture
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 nVertex_ * sizeof(glm::vec2), uvmap, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 2, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;
}

Face::~Face() {
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // delete buffers
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(3, &vbo_[0]);
    // disable attrs
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int Face::getVertexCount() {
    return nVertex_;
}

bool pointInPolygon(std::vector<glm::vec2> v, glm::vec2 p) {
    for (std::size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << std::endl;
//    GLfloat testy = p.y;
//    GLint count = 0;
//    for (std::size_t i = 0; i < v.size(); ++i) {
//        glm::vec2 v1 = v[i];
//        glm::vec2 v2 = v[(i + 1) % v.size()];
//        count += ((v1.y - testy) * (v2.y - testy) < 0);
//    }
//    return count % 2;
    // std::cout << "11" << std::endl;
    GLfloat res = 0, area = 0;
    for (std::size_t i = 0; i < v.size(); i++) {
        glm::vec3 v1 = glm::vec3(v[i] - p, 0);
        glm::vec3 v2 = glm::vec3(v[(i + 1) % v.size()] - p, 0);
        res += glm::cross(v1, v2).z;
        area += glm::abs(glm::cross(v1, v2).z);
    }
    res = glm::abs(res);
    std::cout << res << " " << area << std::endl;
    bool out = glm::abs(res - area) < glm::epsilon<GLfloat>();
    return out;
}

//
//    b      |     b  ZY
//   /  XYZ  |      \
//  /        |       \
// +------a  |  b2----+a
// find a rotation to bring the face to the xy-plane:
//     rotate a to x-azis
//     rotate b around x to fall in xy
// apply to face and line
// extend test line to intersect xy-plane
//     intersection s = off + alpha * dir
//     s.z = off.z + alpha * dir.z = 0
//     alpha = -off.z / dir.z
// 2D point polygon inclusion check

bool Face::intersects(glm::vec3 off, glm::vec3 dir) {

    // std::cout << std::endl;
    // translate
    // std::cout << "1" << std::endl;
    //
    // std::cout << "2" << std::endl;
    glm::vec3 angle;
    glm::mat4 trans = glm::mat4(1);
    glm::vec3 a = glm::normalize(vertex_[0] - vertex_[2]);
    glm::vec3 b = glm::normalize(vertex_[1] - vertex_[2]);
    std::cout << "HA" << std::endl;
    std::cout << "a,b=" << a << b << "" << std::endl;
//    std::cout << "off=" << off << "dir=" << dir << std::endl;
//    // rotate a to x-azis
//    // std::cout << "3" << std::endl;
//    //glm::detail::tvec3<bool> axp = glm::equal(a, glm::vec3(1, 0, 0));
//    //glm::detail::tvec3<bool> axn = glm::equal(a, glm::vec3(-1, 0, 0));
//    angle.x = -glm::degrees(acos(glm::dot(a, glm::vec3(1, 0, 0))));
//    if (!glm::epsilonEqual(glm::sin(glm::radians(angle.x)), 0, glm::epsilon<GLfloat>())) {
//        // std::cout << angle.x << glm::sin(glm::radians(angle.x)) << std::endl;
//        glm::vec3 axisX = glm::cross(a, glm::vec3(1, 0, 0));
//        glm::mat4 rotX = glm::rotate(glm::mat4(1), angle.x, axisX);
//        a = glm::vec3(rotX * glm::vec4(a, 0));
//        b = glm::vec3(rotX * glm::vec4(b, 0));
//        trans = rotX * trans;
//    }
//    // rotate b around x to fall in xy
//    // std::cout << "4" << std::endl;
//    angle.y = -glm::degrees(atan2(b.z, b.y));
//    // std::cout << "5" << std::endl;
//    if (!glm::epsilonEqual(glm::sin(glm::radians(angle.y)), 0, glm::epsilon<GLfloat>())) {
//        glm::mat4 rotYZ
//            = glm::rotate(glm::mat4(1), angle.y, glm::vec3(1, 0, 0));
//        a = glm::vec3(rotYZ * glm::vec4(a, 0));
//        b = glm::vec3(rotYZ * glm::vec4(b, 0));
//        trans = rotYZ * trans;
//    }

    // move z's so that they overlap
    glm::vec3 worldZ = glm::vec3(0, 0, 1);
    glm::vec3 faceZ = glm::normalize(glm::cross(a, b));
    if (glm::dot(faceZ, worldZ) < 0) {
        glm::vec3 t = a;
        a = b;
        b = t;
        faceZ *= -1;
    }
    GLfloat angleZ = glm::acos(glm::dot(faceZ, worldZ));
    std::cout << a << b << glm::cos(angleZ) << std::endl;
    glm::detail::tvec3<bool> noRotation = glm::equal(faceZ, worldZ);
    std::cout << noRotation.x << noRotation.y << noRotation.z << std::endl;
    if (glm::dot(faceZ, worldZ) < 1 - glm::epsilon<GLfloat>() * 10) {
        glm::vec3 axisZ = glm::cross(faceZ, worldZ);
        std::cout << glm::sin(angleZ) << worldZ << faceZ << axisZ << std::endl;
        trans = glm::rotate(trans, -glm::degrees(angleZ), axisZ);
        a = glm::vec3(trans * glm::vec4(a, 0));
        b = glm::vec3(trans * glm::vec4(b, 0));
    }
    std::cout << "a,b=" << a << b << "" << std::endl;
    // apply to line
    // std::cout << "6" << std::endl;
    off = glm::vec3(trans * glm::vec4(off, 0));
    dir = glm::vec3(trans * glm::vec4(dir, 0));
    std::cout << "off=" << off << "dir=" << dir << std::endl;
    // alpha = -off.z / dir.worldZ
    // std::cout << "7" << std::endl;
    if (glm::abs(dir.z) > glm::epsilon<GLfloat>()) {
        GLfloat alpha = -off.z / dir.z;
        // find intersection
        // std::cout << "8" << std::endl;
        glm::vec2 s = glm::vec2(off + alpha * dir);
        std::cout << "s=" << s << std::endl;
        // 2D point polygon inclusion check
        // std::cout << "9" << std::endl;
        std::vector<glm::vec2> rotatedFace(3);
        for (std::size_t i = 0; i < vertex_.size(); ++i)
            rotatedFace[i] = glm::vec2(trans * glm::vec4(vertex_[i], 0));
        // std::cout << "10" << std::endl;
        return pointInPolygon(rotatedFace, s);
    }
    // intersect at infinity
    else
        return false;
}

void Face::render() {
    glBindVertexArray(vao_);
    glDrawArrays(drawMode_, 0, nVertex_);
}

}
