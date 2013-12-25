#ifndef MESH_H_
#define MESH_H_

#include "../Internal.h"
#include "Resource.h"
#include "Face.h"

namespace game {

struct vertex {
  float x, y, z;
  float nx, ny, nz;
  float s0, t0;
};

class Mesh : public Resource {
    GLuint vao_, vbo_[4], draw_mode_, vertex_count_, buffers_;
    std::vector<Face *> faces_;
public:
    Mesh(std::string uid, std::vector<glm::vec3> v, std::vector<glm::vec3> vt,
         std::vector<glm::vec3> vn, std::vector<GLuint> idx, GLint draw_mode);
    ~Mesh();

    void render(glm::mat4 transformation);

    GLboolean getIntersect(glm::vec3 off, glm::vec3 dir) {
        glm::vec3 s;
        for (std::size_t i = 0; i < faces_.size(); ++i)
            if (faces_[i] && faces_[i]->intersect(off, dir, &s))
                return true;
        return false;
    }

    glm::vec3 getNearestIntersect(glm::vec3 off, glm::vec3 dir) {
        glm::vec3 s, nearestS;
        GLfloat minDist = std::numeric_limits<GLfloat>::infinity();
        for (std::size_t i = 0; i < faces_.size(); ++i)
            if (faces_[i] && faces_[i]->intersect(off, dir, &s)) {
                GLfloat dist = glm::dot(off - s, dir);
                if (dist > 0 && dist < minDist) {
                    minDist = dist;
                    nearestS = s;
                }
             }
        return nearestS;
        return glm::vec3(-1, -1, -1);
    }
};

}

#endif
