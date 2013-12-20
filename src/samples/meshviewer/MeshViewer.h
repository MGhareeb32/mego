#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include "../../game/Game.h"
#include "MeshViewer.h"

class MeshViewer : public game::MeshEntity {
    game::MeshEntity *room_entity_, *obj_entity_, *light_entity_;
    std::vector<game::MeshEntity> *map_entities_;
    game::Camera *cam_;
    game::Light *light_;
public:
    MeshViewer();
    ~MeshViewer();
    void update();
};

#endif
