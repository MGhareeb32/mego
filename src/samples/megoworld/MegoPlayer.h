#ifndef MEGOPLAYER_H_
#define MEGOPLAYER_H_

#include "../../game/Game.h"
#include "Grid.h"

class MegoPlayer : public game::Camera {
    Grid *grid_;
public:
    MegoPlayer(Grid *grid_);
    virtual ~MegoPlayer();

    void jump();
    void walk(glm::vec3 d, GLboolean speed);
};

#endif
