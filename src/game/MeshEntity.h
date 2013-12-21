#ifndef MESHENTITY_H_
#define MESHENTITY_H_

#include "Entity.h"
#include "resource/ResMgr.h"

namespace game {

class MeshEntity : public Entity {
    Mesh *mesh_;
    Material *mtl_;
public:
    MeshEntity(Mesh *m = NULL) : Entity() { mesh_ = m, mtl_ = NULL; };
    MeshEntity(std::string mesh, std::string mtl = "",
               std::string texture = "") : Entity() {
        mesh_ = (Mesh*)ResMgr::load(mesh);
        if (mtl.size())
            mtl_ = (Material*)ResMgr::load(mtl);
        if (texture.size())
            mtl_->set_texture((Texture*)ResMgr::load(texture));
    };
    ~MeshEntity() {};

    Mesh* mesh() { return mesh_; }
    void set_mesh(Mesh *m) { mesh_ = m; }
    Material* mtl() { return mtl_; }
    void set_mtl(Material *mtl) { mtl_ = mtl; }

    virtual void render() {
        // render self
        if (mtl_)
            mtlSet(mtl_);
        if (mesh_)
            mesh_->render(transform());
        // super implementation
        Entity::render();
    }

    GLboolean getIntersect(glm::vec3 off, glm::vec3 dir) {
        if (mesh_) {
            glm::mat4 m = transform_i();
            off = glm::vec3(m * glm::vec4(off, 1));
            dir = glm::normalize(glm::vec3(m * glm::vec4(dir, 0)));
            return mesh_->getIntersect(off, dir);
        }
        return false;
    }

    glm::vec3 getNearestIntersect(glm::vec3 off, glm::vec3 dir) {
        if (mesh_) {
            glm::mat4 m = transform_i();
            off = glm::vec3(m * glm::vec4(off, 1));
            dir = glm::normalize(glm::vec3(m * glm::vec4(dir, 0)));
            glm::mat4 m_i = transform();
            glm::vec3 s = mesh_->getNearestIntersect(off, dir);
            return glm::vec3(m_i * glm::vec4(s, 1));
        }
    }
};

}

#endif
