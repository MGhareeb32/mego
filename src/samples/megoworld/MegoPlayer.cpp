#include "MegoPlayer.h"

MegoPlayer::MegoPlayer(Grid *grid) : grid_(grid),
    RD(.5f * grid_->SZ.x), HT(1.5f * grid_->SZ.z), SPEED(.1f) {

    eye_ = new game::Camera();
    eye_->lookAt(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    addChild("eye", eye_);

    // align camera
    eye_->persp(70, game::screen_size().x / game::screen_size().y);
    eye_->lookAt(glm::vec3(0, 0, 0),
                 glm::vec3(1.f, .5f, 0),
                 glm::vec3(0, 0, 1));
    eye_->translate(glm::vec3(0.f, 0.f, 0.04f));
    translate(grid_->indexToWorld(glm::ivec3(0, 0, 1)) + .5f * Grid::SZ);

    ht_ = falling_ = zspeed_ = 0;
    //crouching_ = 1;

    item_selected_ = -1;

    highlight_brick_
        = (game::Mesh *)game::ResMgr::load
            ("res/megoworld/mego-brick-highlight.obj");
    highlight_brick_mtl_
        = new game::Material(*(game::Material *)game::ResMgr::load
            ("res/lamp.mtl"));
    highlight_brick_mtl_->set_texture((game::Texture *)game::ResMgr::load
            ("res/megoworld/mego-brick-highlight.png"));
    highlight_pos_ = glm::ivec3(-1, -1, -1);

    // arm
    game::MeshEntity *arm
        = new game::MeshEntity("res/megoworld/mego-man-arm.obj",
                               "res/megoworld/mego-man-skin.mtl");
    arm->scale(Grid::SZ);
    arm->scale(.5f * glm::vec3(1, 1, 2));
    arm->rotate(-100, glm::vec3(0, 0, 1));
    arm->rotate(55, glm::vec3(-1, 1, 0));
    arm->translate(glm::vec3(.12f, .01f, 0.04f));
    eye_->addChild("rarm", arm);

    translate(grid_->indexToWorld(glm::ivec3(0, 0, -1)) + .5f * Grid::SZ);
    translate(grid_->indexToWorld(grid_->spawn_point()) + .5f * Grid::SZ);
}

MegoPlayer::~MegoPlayer() {
}

void MegoPlayer::render() {
    if (highlight_pos_.x >= 0) {
        game::mtlSet(highlight_brick_mtl_);
        highlight_brick_->render(grid_->brick_trans(highlight_pos_));
    }
    game::MeshEntity::render();
}

void MegoPlayer::update() {
    // move
    GLfloat speed = 3.f - 2.f * (game::key_mod_ & GLUT_ACTIVE_SHIFT);
    if (game::key_down_['w'])
        walkForward(-speed);
    if (game::key_down_['s'])
        walkForward(+speed);
    if (game::key_down_['a'])
        walkSideways(-speed);
    if (game::key_down_['d'])
        walkSideways(+speed);

    // item select
    if (game::key_down_['1'])
        item_selected_ = 1;
    if (game::key_down_['2'])
        item_selected_ = 2;
    if (game::key_down_['3'])
        item_selected_ = 3;
    if (game::key_down_['4'])
        item_selected_ = 4;
    if (game::key_down_['5'])
        item_selected_ = 5;
    if (game::key_down_['6'])
        item_selected_ = 6;
    if (game::key_down_['7'])
        item_selected_ = 7;

    // jump
    if (game::key_press_[' '])
        jump();

    // crouch
    GLfloat prev_ht = ht_;
    ht_ += (.8f * (1 /*+ !crouching_*/) * HT - ht_) * .5;
//    eye_->translate();

    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    eye_->transform(eye_->fpsRotation(speed * delta, GL_FALSE, GL_FALSE));
    eye_->translate(-eye_->o());
    eye_->translate(o() + glm::vec3(0.f, 0.f, 0.04f + .8f * (1 /*+ !crouching_*/) * HT));

    // gravity
    falling_ = placeFree(o() + glm::vec3(0, 0, zspeed_ - ht_ * .3f));
    zspeed_ = glm::clamp(zspeed_ + Grid::GRAVITY * falling_, -HT, HT);
    if (glm::abs(zspeed_) > glm::epsilon<GLfloat>())
        translate(glm::vec3(0, 0, zspeed_));

    // resolve collision
    GLboolean stuck = true;
    for (GLfloat i = 0; i <= RD * 2 && stuck; i += .1f * RD * SPEED)
        for (GLint j = 0; j < 6 && stuck; ++j)
            if (placeFree(o() + Grid::DIR[j] * i)) {
                translate(Grid::DIR[j] * i), stuck = false;
                if (glm::abs(i) >  glm::epsilon<GLfloat>())
                    zspeed_ = 0.f;
            }

    // pointing
    glm::vec3 pointInter;
    highlight_pos_ = gridLocalPointBrick(&pointInter);
    if (highlight_pos_.x >= 0) {
        if (game::mouse_click_[GLUT_LEFT_BUTTON])
            pickBrick(highlight_pos_);
        if (game::mouse_click_[GLUT_RIGHT_BUTTON])
            putBrick(highlight_pos_, pointInter);
    }

    grid_->set_detail_center(grid_->worldToIndex(o()));
}

void MegoPlayer::walk(glm::vec3 v, GLfloat speed) {
    // new position
    v.z = 0;
    v = SPEED * RD * speed * glm::normalize(v);
    // x
    glm::vec3 dX = glm::vec3(v.x, 0, 0);
    glm::vec3 nextOX = o() + dX;
    if (placeFree(nextOX))
        translate(dX);
    // y
    glm::vec3 dY = glm::vec3(0, v.y, 0);
    glm::vec3 nextOY = o() + dY;
    if (placeFree(nextOY))
        translate(dY);
}

void MegoPlayer::jump() {
    if (!falling_)
        zspeed_ = SPEED * .28f;
}

void MegoPlayer::pickBrick(glm::ivec3 worldPointBrick) {
    GLint brick = grid_->worldBrickHit(worldPointBrick);
    if (brick > 0)
        item_count_[brick]++;
}

void MegoPlayer::putBrick(glm::ivec3 worldPointBrick, glm::vec3 pointInter) {

    GLint item = item_selected_;
    if (!item_count_.size())
        return;
    if (item == -1)
        item = item_count_.begin()->first;

    if (item_count_.find(item) != item_count_.end() && item_count_[item] > 0) {
        // placed successfully
        glm::ivec3 cell
            = grid_->localBrickPlace(worldPointBrick, pointInter, item);
        if (cell.x > 0) {
            item_count_[item]--;
            // failed
            if (!placeFree(o())) {
                grid_->localBrickPlace(cell, Grid::CELL_EMPTY);
                item_count_[item]++;
            }
            // erase when empty
            if (item_count_[item] <= 0)
                item_count_.erase(item);
        }
    }
}
