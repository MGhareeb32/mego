#ifndef MEGOWORLD_H_
#define MEGOWORLD_H_

#include "../../game/Game.h"
#include "Grid.h"
#include "MegoPlayer.h"

class MegoWorld : public game::MeshEntity {
    Grid *grid_;
    MegoPlayer *player_;

    game::MeshEntity*light_entity_[game::NUM_LIGHTS];
    game::Light *light_[game::NUM_LIGHTS];
public:
    MegoWorld();
    ~MegoWorld();
    void update();
};

#endif
