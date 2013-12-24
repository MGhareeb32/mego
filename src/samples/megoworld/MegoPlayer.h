#ifndef MEGOPLAYER_H_
#define MEGOPLAYER_H_

#include "../../game/Game.h"
#include "Grid.h"

class MegoPlayer : public game::MeshEntity {

    Grid *grid_;

    const GLfloat RD, HT, SPEED;
    GLboolean falling_;//, crouching_;
    GLfloat ht_, zspeed_;

    GLint item_selected_;
    std::map<GLint, GLint> item_count_;

    game::Camera *eye_;

public:

    MegoPlayer(Grid *grid_);
    virtual ~MegoPlayer();

    void update();

    game::Camera *eye() { return eye_; }

    GLboolean placeFree(glm::vec3 v) {
        for (GLint i = 0; i < 3; i++) {
            glm::vec3 vFoot = v + grid_->indexToWorld(glm::ivec3(0, 0, i));
            vFoot.z += Grid::SZ.z;
            for (int i = 0; i < 6; ++i)
                if (grid_->worldCell
                        (vFoot + Grid::DIR[i] * .4f * Grid::SZ) > 0)
                    return false;
        }
        return true;
    }

    glm::ivec3 gridLocalPointBrick(glm::vec3 *pointInter) {
        glm::ivec3 nearestCell = glm::ivec3(-1, -1, -1);
        // TODO make static
        game::MeshEntity *testBrick
            = new game::MeshEntity("res/megoworld/mego-brick-1.obj");
        std::vector<glm::ivec3> cells = grid_->worldCloseCells(o());
        glm::vec3 p;
        GLfloat minDist = std::numeric_limits<GLfloat>::infinity();
        glm::ivec3 pointCell;
        for (std::size_t i = 0; i < cells.size(); ++i) {
            // translate
            glm::ivec3 cell = cells[i];
            testBrick->resetTransform();
            testBrick->transform(glm::translate(Grid::SCALE, glm::vec3(cell)));
            // test for intersection
            if (testBrick->getIntersect(eye_->o(), eye_->n())) {
                p = testBrick->getNearestIntersect(eye_->o(), eye_->n());
                // makes sure it's in front of the camera
                GLfloat cosAngle
                    = glm::dot(glm::normalize(eye_->o() - p), eye_->n());
                GLfloat dist = glm::distance(eye_->o(), p);
                if (cosAngle > 0 && dist < minDist) {
                    minDist = dist;
                    nearestCell = cell;
                    pointInter->x = p.x;
                    pointInter->y = p.y;
                    pointInter->z = p.z;
                }
            }
        }
        delete testBrick;
        return nearestCell;
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

    // bricks

    void pickBrick(glm::ivec3 worldPointBrick);
    void putBrick(glm::ivec3 worldPointBrick, glm::vec3 pointInter);
};

#endif
