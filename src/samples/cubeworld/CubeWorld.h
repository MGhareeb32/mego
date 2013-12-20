#ifndef CUBEWORLD_H_
#define CUBEWORLD_H_

#include "../../game/Game.h"

class CubeWorld : public game::MeshEntity {
    game::MeshEntity *light_entity_;
    game::Camera *cam_;
    game::Light *light_;
    std::vector<game::MeshEntity *> map_entities_;
public:
    CubeWorld();
    ~CubeWorld();
    void update();
};

#endif
