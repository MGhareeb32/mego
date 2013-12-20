#ifndef CUBEWORLD_H_
#define CUBEWORLD_H_

#include "../../game/Game.h"
#define N_LIGHT 1

class CubeWorld : public game::MeshEntity {
    game::MeshEntity *room_entity_, *obj_entity_, *light_entity_[4];
    game::Camera *cam_;
    game::Light *light_[N_LIGHT];
public:
    CubeWorld();
    ~CubeWorld();
    void update();
};

#endif
