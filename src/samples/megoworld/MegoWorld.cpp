#include "MegoWorld.h"

MegoWorld::MegoWorld() {
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    game::sceneColorSet(glm::vec3(0.1f, 0.1f, 0.1f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    // load map
    grid_ = new Grid("res/megoworld/map.in");
    addChild("grid", grid_);
    // camera
    player_ = new MegoPlayer(grid_);
    game::cameraSet(player_->eye());
    // light
    glm::ivec3 size = grid_->size();
    GLint lightArea = size.x * size.y / game::NUM_LIGHTS;
    GLint lightAreaLen = glm::sqrt(lightArea) + 1;
    GLint nLightX = (size.x) / lightAreaLen;
    GLint nLightY = (size.y) / lightAreaLen;
    for (int i = 0; i < nLightX * nLightY; ++i) {
        GLfloat x = lightAreaLen * ((i % nLightX) + .5f) - .5f * lightAreaLen;
        GLfloat y = lightAreaLen * ((i / nLightX) + .5f) - .5f * lightAreaLen;
        // light
        light_[i] = new game::Light(glm::vec3(1.f / nLightX / nLightY));
        std::stringstream ss;
        ss << "light" << i;
        addChild(ss.str(), light_[i]);
        game::lights.push_back(light_[i]);
        // model
        light_entity_[i] = new game::MeshEntity("res/lamp.obj",
                                                "res/lamp.mtl");
        light_entity_[i]->scale(glm::vec3(.05f, .05f, .05f));
        light_[i]->addChild("light", light_entity_[i]);
        light_[i]->translate(glm::vec3(x, y, size.z + 1.5f));
    }
    scale(glm::vec3(.4f, .4f, .4f));
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

void MegoWorld::checkGameEnd(){
    // win
    if (grid_->worldCell(grid_->getTarget()) != Grid::CELL_TARGET){
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
    player_->update();
    grid_->updateTimer();
    updateTitle();
    checkGameEnd();
}
