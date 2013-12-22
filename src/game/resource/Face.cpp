#include "Face.h"

namespace game {

Face::Face(GLint vertex_count, glm::vec3 *vertex, glm::vec2 *uvmap,
           glm::vec3 *normal, GLint mode) {

    drawMode_ = mode;
    vertex_count_ = vertex_count;

    //
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(3, &vbo_[0]);

    GLint attr_idx = 0;

    // vertex position to buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 vertex_count_ * sizeof(glm::vec3), vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;

    // vertex normal to buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 vertex_count_ * sizeof(glm::vec3), normal, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;

    // vertex texture to buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[attr_idx]);
    glBufferData(GL_ARRAY_BUFFER,
                 vertex_count_ * sizeof(glm::vec2), uvmap, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attr_idx);
    glVertexAttribPointer(attr_idx, 2, GL_FLOAT, GL_FALSE, 0, 0);
    attr_idx++;

    // intersection checks pre-calculation
    // find xy-plane equivalent
    glm::vec3 faceZ = glm::normalize(glm::cross(vertex[0] - vertex[2],
                                                vertex[1] - vertex[2]));
    glm::vec3 worldZ = glm::vec3(0, 0, 1);
    trans_i_ = glm::translate(glm::mat4(1), -vertex[2]);
    // rotate so that normal overlaps with z axis
    GLfloat cosAngleZ = glm::dot(faceZ, worldZ);
    if (glm::abs(cosAngleZ) <= 1 - glm::epsilon<GLfloat>()) {
        glm::vec3 axisZ = glm::cross(faceZ, worldZ);
        GLfloat s = glm::sqrt((1 + cosAngleZ) * 2);
        GLfloat invs = 1 / s;
        glm::mat4 rot = glm::toMat4(glm::quat(s * .5f, axisZ * invs));
        trans_i_ = rot * trans_i_;
    }
    trans_ = glm::inverse(trans_i_);
    xyPlane_vertex_ = new glm::vec3[vertex_count_ + 1];
    for (GLint i = 0; i < vertex_count_; ++i)
        xyPlane_vertex_[i] = glm::vec3(trans_i_ * glm::vec4(vertex[i], 1));
    xyPlane_vertex_[vertex_count_] = xyPlane_vertex_[0];
}

Face::~Face() {
    delete[] xyPlane_vertex_;
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

GLboolean Face::intersect(glm::vec3 off, glm::vec3 dir, glm::vec3 *s) {
    // go to local face coordinate system
    dir = glm::vec3(trans_i_ * glm::vec4(dir, 0));
    if (glm::abs(dir.z) > glm::epsilon<GLfloat>()) {
        off = glm::vec3(trans_i_ * glm::vec4(off, 1));
        // find line-xyplane intersection
        GLfloat alpha = -off.z / dir.z;
        glm::vec3 localS = off + alpha * dir;
        *s = glm::vec3(trans_ * glm::vec4(localS, 1));
        // 2D point polygon inclusion check
        return xyPlaneFaceContains(localS);
    }
    // parallel
    else
        return false;
}

void Face::render() {
    glBindVertexArray(vao_);
    glDrawArrays(drawMode_, 0, vertex_count_);
}

}
