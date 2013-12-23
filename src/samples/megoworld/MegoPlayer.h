#ifndef MEGOPLAYER_H_
#define MEGOPLAYER_H_

#include "../../game/Game.h"
#include "Grid.h"

class MegoPlayer : public game::MeshEntity {

    Grid *grid_;

    const GLfloat HT, RD, SPEED;
    GLboolean falling_, crouching_;
    GLfloat ht_, zspeed_;


    game::Camera *eye_;

public:

    MegoPlayer(Grid *grid_);
    virtual ~MegoPlayer();

    void update();

    game::Camera *eye() { return eye_; }

    GLboolean placeFree(glm::vec3 v) {
//        for (GLfloat i = Grid::SZ.z / 2; i < HT - Grid::SZ.z; i += Grid::SZ.z) {
//            glm::vec3 v2 = v + glm::vec3(0, 0, 1) * i * Grid::SZ.z;
//            for (int i = 0; i < 12; ++i)
//                if (grid_->worldCell(v2 + Grid::DIR[i] * .23f * RD) > 0)
//                return false;
//        }
//        glm::vec3 vFoot = v + glm::vec3(0, 0, Grid::SZ.z);
//        for (GLint j = 0; j < 6; ++j)
//            if (grid_->worldCell(vFoot + Grid::DIR[j] * .23f * Grid::SZ)
//                    != grid_->CELL_EMPTY)
//                return false;
//        glm::vec3 vHead = v + glm::vec3(0, 0, RD * 3);
//        for (GLint j = 0; j < 6; ++j)
//            if (grid_->worldCell(vHead + Grid::DIR[j] * .23f * RD)
//                    != grid_->CELL_EMPTY)
//                return false;
        for (GLfloat i = .5f * Grid::SZ.z; i <= ht_ + Grid::SZ.z / 2; i += Grid::SZ.z) {
        glm::vec3 vFoot = v + glm::vec3(0, 0, i);
            for (int i = 0; i < 12; ++i)
                if (grid_->worldCell(vFoot + Grid::CRNR[i] * .2f * Grid::SZ) > 0)
                    return false;
        }
        return true;
    }

    // walk

    void walk(glm::vec3 v, GLfloat speed);

    void walkForward(GLfloat speed) {
        walk(eye_->n(), speed);
    }

    void walkSideways(GLfloat speed) {
        walk(eye_->u(), speed);
    }

    // jump

    void jump();
};

#endif
