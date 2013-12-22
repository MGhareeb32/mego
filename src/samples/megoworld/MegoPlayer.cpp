#include "MegoPlayer.h"

MegoPlayer::MegoPlayer(Grid *grid) : grid_(grid) {
    persp();
    lookAt(glm::vec3(0, 0, 0), glm::vec3(1.f, .5f, 0), glm::vec3(0, 0, 1));
    translate(grid_->indexToWorld(grid_->spawn_point()));
    std::cout << grid_->indexToWorld(grid_->spawn_point()) << std::endl;
    std::cout << o() << std::endl;
}

MegoPlayer::~MegoPlayer() {
}

void MegoPlayer::jump() {

}

void MegoPlayer::walk(glm::vec3 d, GLboolean speed) {

}
