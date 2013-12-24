#ifndef MESHVIEWER_H_
#define MESHVIEWER_H_

#include "../../game/Game.h"


class MeshViewer : public game::MeshEntity {
    static const GLint N_LIGHT = 3;

    game::Entity *lights_;
    game::MeshEntity *room_entity_, *obj_entity_;
    game::Camera *cam_;
public:
    MeshViewer();
    ~MeshViewer();
    void update();
};

#endif
