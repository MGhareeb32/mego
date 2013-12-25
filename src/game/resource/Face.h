#ifndef FACE_H_
#define FACE_H_

#include "../OpenGL.h"
#include "../internal.h"
#include "Material.h"

namespace game {

bool pointInPolygon(std::vector<glm::vec2> v, glm::vec2 p);

class Face {
protected:
    GLint vertex_count_;
    glm::vec3 *xyPlane_vertex_;
    glm::mat4 trans_, trans_i_;

    GLboolean xyPlaneFaceContains(glm::vec3 p) {
        GLfloat res = 0, area = 0;
        for (int i = 0; i < vertex_count_ ; i++) {
            glm::vec3 v1 = xyPlane_vertex_[i] - p;
            glm::vec3 v2 = xyPlane_vertex_[i + 1] - p;
            res += glm::cross(v1, v2).z;
            area += glm::abs(glm::cross(v1, v2).z);
        }
        return glm::abs(glm::abs(res) - area) <= glm::epsilon<GLfloat>();
    }

public:
    Face(GLint vertex_count, glm::vec3 *vertex) {
        vertex_count_ = vertex_count;

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

    ~Face() {
        delete[] xyPlane_vertex_;
    }


    GLboolean intersect(glm::vec3 off, glm::vec3 dir, glm::vec3 *s) {
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
};

}

#endif
