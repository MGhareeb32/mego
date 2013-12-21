#ifndef FACE_H_
#define FACE_H_

#include "../OpenGL.h"
#include "../internal.h"
#include "Material.h"

namespace game {

bool pointInPolygon(std::vector<glm::vec2> v, glm::vec2 p);

class Face {
private:
    int nVertex_;
    std::vector<glm::vec3> vertex_;
    GLuint vao_, vbo_[3], drawMode_;
public:
    Face(int vertexCount, glm::vec3 *vertex, glm::vec2 *uvmap,
         glm::vec3 *normal, GLint mode);
    virtual ~Face();

    int getVertexCount();
    bool intersects(glm::vec3 off, glm::vec3 dir);

    void render();
};

}

#endif
