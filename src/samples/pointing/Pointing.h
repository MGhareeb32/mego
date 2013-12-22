#ifndef POINTING_H_
#define POINTING_H_

#include "../../game/Game.h"

class Pointing : public game::MeshEntity {
    game::MeshEntity *light_entity_;
    game::Camera *cam_;
    game::Light *light_;
    std::vector<game::MeshEntity *> map_entities_;
public:
    Pointing();
    ~Pointing();
    void update();
};

#endif
