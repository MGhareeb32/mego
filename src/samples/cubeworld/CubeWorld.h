#ifndef CUBEWORLD_H_
#define CUBEWORLD_H_

#include "../../game/Game.h"

class CubeWorld : public game::MeshEntity {
    static const GLfloat SZ = .1f;
    game::Camera *cam_;
    game::MeshEntity **light_entity_;
    game::Light **light_;
    std::vector<game::MeshEntity *> map_entities_;
public:
    CubeWorld();
    ~CubeWorld();
    void reset();
    void update();
};

#endif
