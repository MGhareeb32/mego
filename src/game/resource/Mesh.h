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

    bool intersects(glm::mat4 m, glm::vec3 off, glm::vec3 dir) {
        off = glm::vec3(m * glm::vec4(off, 1));
        dir = glm::normalize(glm::vec3(m * glm::vec4(dir, 1)));
        for (std::size_t i = 0; i < faces.size(); ++i)
            if (faces[i] && faces[i]->intersects(off, dir))
                return true;
        return false;
    }
};

}

#endif
