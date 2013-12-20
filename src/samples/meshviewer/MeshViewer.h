#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include "../../game/Game.h"
#include "MeshViewer.h"
#define N_LIGHT 1

class MeshViewer : public game::MeshEntity {
    game::MeshEntity *room_entity_, *obj_entity_, *light_entity_[4];
    game::Camera *cam_;
    game::Light *light_[N_LIGHT];
public:
    MeshViewer();
    ~MeshViewer();
    void update();
};

#endif
