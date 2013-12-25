#include "MegoWorld.h"

const GLint MegoWorld::NUM_LIGHT = 4;

MegoWorld::MegoWorld() {

    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    game::sceneColorSet(glm::vec3(0.1f, 0.1f, .2f));
    game::fogSet(glm::vec4(0.4f, 0.6f, 1.f, 1.f), .15f);
    game::setUniformShowBackface(GL_FALSE);

    // load map
    grid_ = new Grid("res/megoworld/map.in");
    addChild("grid", grid_);
    // camera
    player_ = new MegoPlayer(grid_);
    game::cameraSet(player_->eye(), GL_TRUE);
    addChild("player", player_);

    // light
    glm::ivec3 size = grid_->size();
    GLint lightArea = size.x * size.y / NUM_LIGHT;
    std::cout << lightArea << std::endl;
    GLint lightLen = ceil(sqrt(lightArea));
    lightLen = lightLen <= 0 ? 1 : lightLen;
    GLint nLightX = size.x / lightLen;
    nLightX = nLightX <= 0 ? 1 : nLightX;
    lights_ = new game::Entity();
    for (int i = 0; i < NUM_LIGHT; ++i) {
        int x = lightLen * (i % nLightX);
        int y = lightLen * (i / nLightX);
        // light
        light_[i] = new game::Light(glm::vec3(1.f / NUM_LIGHT));
        game::lights.push_back(light_[i]);
        // mesh
        light_entity_[i] = new game::MeshEntity("res/lamp.obj", "res/lamp.mtl");
        light_entity_[i]->addChild("light", light_[i]);
        light_entity_[i]->scale(glm::vec3(.01f, .01f, .01f));
        light_entity_[i]->translate
            (grid_->indexToWorld(glm::ivec3(x, y, size.z + 32)));
        // add
        std::stringstream ss;
        ss << "light" << i;
        lights_->addChild(ss.str(), light_entity_[i]);
    }
    addChild("all-lights", lights_);
}

MegoWorld::~MegoWorld() {
}

int MegoWorld::calcPlayerTargetDist(){
    glm::vec3 diff = (player_->o() - grid_->getTarget());
    return ((int) (std::sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2]
            * diff[2]) / ((Grid::SZ[0]+Grid::SZ[1])/2)));
}

std::string toTimeFormat(int time){
    int mins = time/60;
    int secs = time%60;
    std::stringstream timeFormat;
    timeFormat << ((mins < 10)?"0":"") << mins
               << ":" << ((secs < 10)?"0":"") << secs;
    return timeFormat.str();
}


void MegoWorld::updateTitle(){
    std::stringstream title;
    title << "You are " << calcPlayerTargetDist()
          << " away from the treasure. " << toTimeFormat(grid_->getTimer())
          << " remaining";
    glutSetWindowTitle(title.str().c_str());
}

void MegoWorld::checkGameEnd() {
    // win
    if (grid_->getTarget().x > 0
        && grid_->worldCell(grid_->getTarget()) != Grid::CELL_TARGET) {
        MessageBox(NULL, "Win GAME!", "SEE U!", 0);
        exit(EXIT_SUCCESS);
    }
    // lose
    if (grid_->getTimer() == 0){
        MessageBox(NULL, "TIME OVER, LOSER!", "SEE U!", 0);
        exit(EXIT_SUCCESS);
    }
}

void MegoWorld::update() {
    updateTitle();
    checkGameEnd();
    Entity::update();
}
