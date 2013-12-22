#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include "../../game/Game.h"


class MeshViewer : public game::MeshEntity {
    static const GLint N_LIGHT = 1;

    game::MeshEntity *room_entity_, *obj_entity_, *light_entity_[4];
    game::Camera *cam_;
    game::Light *light_[N_LIGHT];
public:
    MeshViewer();
    ~MeshViewer();
    void update();
};

#endif
