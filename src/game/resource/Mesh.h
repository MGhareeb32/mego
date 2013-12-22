#ifndef MESH_H_
#define MESH_H_

#include "Face.h"
#include "../Internal.h"
#include "Resource.h"

namespace game{

class Mesh : public Resource {
    std::vector<Face *> faces;
public:
    Mesh(std::string uid) : Resource(uid) {};

    ~Mesh() {
        for (std::size_t i = 0; i < faces.size(); ++i)
            faces[i]->~Face();
    }

    void addFace(Face *f) { faces.push_back(f); }

    void render(glm::mat4 transformation) {
        game::setUniformModelMatrix(transformation);
        for (std::size_t i = 0; i < faces.size(); ++i)
            faces[i]->render();
    }

    GLboolean getIntersect(glm::vec3 off, glm::vec3 dir) {
        glm::vec3 s;
        for (std::size_t i = 0; i < faces.size(); ++i)
            if (faces[i] && faces[i]->intersect(off, dir, &s))
                return true;
        return false;
    }

    glm::vec3 getNearestIntersect(glm::vec3 off, glm::vec3 dir) {
        glm::vec3 s, nearestS;
        GLfloat minDist = std::numeric_limits<GLfloat>::infinity();
        for (std::size_t i = 0; i < faces.size(); ++i)
            if (faces[i] && faces[i]->intersect(off, dir, &s)) {
                GLfloat dist = glm::dot(off - s, dir);
                if (dist > 0 && dist < minDist) {
                    minDist = dist;
                    nearestS = s;
                }
             }
        return nearestS;
    }
};

}

#endif
