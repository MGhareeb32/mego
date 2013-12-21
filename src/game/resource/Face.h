#ifndef FACE_H_
#define FACE_H_

#include "../OpenGL.h"
#include "../internal.h"
#include "Material.h"

namespace game {

bool pointInPolygon(std::vector<glm::vec2> v, glm::vec2 p);

class Face {
private:
    GLuint vao_, vbo_[3], drawMode_;

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
    Face(GLint vertex_count, glm::vec3 *vertex, glm::vec2 *uvmap,
         glm::vec3 *normal, GLint mode);
    virtual ~Face();

    GLboolean intersect(glm::vec3 off, glm::vec3 dir, glm::vec3 *s);

    void render();
};

}

#endif
