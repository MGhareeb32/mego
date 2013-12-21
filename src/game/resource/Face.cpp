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

bool containsOrigin(glm::vec2 *v, std::size_t size, glm::vec2 s) {
    GLfloat res = 0, area = 0;
    for (std::size_t i = 0; i < size; i++) {
        glm::vec3 v1 = glm::vec3(v[i] - s, 0);
        glm::vec3 v2 = glm::vec3(v[(i + 1) % size] - s, 0);
        res += glm::cross(v1, v2).z;
        area += glm::abs(glm::cross(v1, v2).z);
    }
    return glm::abs(glm::abs(res) - area) <= glm::epsilon<GLfloat>();
}


// bring one corner to the origin
// rotate so that normal overlaps with z axis
// face is now flat in xy plane
// find line-xyplane intersection
// 2D point polygon inclusion check

bool Face::intersects(glm::vec3 off, glm::vec3 dir) {
    // find face normal
    glm::vec3 a = vertex_[0] - vertex_[2];
    glm::vec3 b = vertex_[1] - vertex_[2];
    glm::vec3 worldZ = glm::vec3(0, 0, 1);
    glm::vec3 faceZ = glm::normalize(glm::cross(a, b));
//    if (glm::dot(faceZ, worldZ) < 0) {
//        glm::vec3 t = b;
//        b = a;
//        a = t;
//        faceZ *= -1;
//    }
    // bring one corner to the origin
    glm::mat4 trans = glm::translate(glm::mat4(1), -vertex_[2]);
    // rotate so that normal overlaps with z axis
    GLfloat angleZ = glm::acos(glm::dot(faceZ, worldZ));
    if (glm::abs(glm::dot(faceZ, worldZ)) <= 1 - glm::epsilon<GLfloat>()) {
        glm::vec3 axisZ = glm::cross(faceZ, worldZ);
        glm::mat4 rot = glm::rotate(glm::mat4(1), -glm::degrees(angleZ), axisZ);
        trans = rot * trans;
    }
    // face is now flat in xy plane
    if (glm::abs(dir.z) > glm::epsilon<GLfloat>()) {
        a = glm::vec3(trans * glm::vec4(a, 0));
        b = glm::vec3(trans * glm::vec4(b, 0));
        off = glm::vec3(trans * glm::vec4(off, 1));
        dir = glm::vec3(trans * glm::vec4(dir, 0));
        GLfloat alpha = -off.z / dir.z;
        // find line-xyplane intersection
        glm::vec2 s = glm::vec2(off + alpha * dir);
        std::cout << "a,b,s=" << a << b << (off + alpha * dir) << std::endl;
//        std::cout << s << std::endl;
        // 2D point polygon inclusion check
        std::cout << s << " ";
        glm::vec2 rotatedFace[vertex_.size()];
        for (std::size_t i = 0; i < vertex_.size(); ++i)
            rotatedFace[i] = glm::vec2(trans * glm::vec4(vertex_[i], 1)),
            std::cout << rotatedFace[i] << " ";
        std::cout << std::endl;
        return containsOrigin(rotatedFace, vertex_.size(), s);
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
