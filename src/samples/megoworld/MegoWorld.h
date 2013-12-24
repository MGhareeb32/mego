#ifndef MEGOWORLD_H_
#define MEGOWORLD_H_

#include "../../game/Game.h"
#include "Grid.h"
#include "MegoPlayer.h"

class MegoWorld : public game::MeshEntity {
    static const GLint NUM_LIGHT;

    Grid *grid_;
    MegoPlayer *player_;

    game::Entity *lights_;
    game::MeshEntity*light_entity_[game::NUM_LIGHTS];
    game::Light *light_[game::NUM_LIGHTS];

    int calcPlayerTargetDist();
    void checkGameEnd();
    void updateTitle();
public:
    MegoWorld();
    ~MegoWorld();
    void update();
};

#endif
