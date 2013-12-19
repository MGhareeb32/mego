#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include "../../game/Game.h"
#include "MeshViewer.h"

class MeshViewer : public game::MeshEntity {
    game::Mesh *axes_mesh_, *obj_mesh_, *light_mesh_;
    game::Material *axes_mtl_, *obj_mtl_, *light_mtl_;
    game::MeshEntity /**obj_entity_, */*light_entity_;
    // TODO add mapEntities
    std::vector<game::MeshEntity> *map_entities_;
    game::Light *light_;
    game::Camera *cam_;
public:
    MeshViewer();
    ~MeshViewer();
    void update();
};

#endif
