#include "MegoWorld.h"

const GLint MegoWorld::NUM_LIGHT = 4;

MegoWorld::MegoWorld() {

    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    game::sceneColorSet(glm::vec3(0.05f, 0.05f, .05f));
    game::fogSet(glm::vec4(0.5f, 0.8f, 1.f, 1.f), .03f);
    game::setUniformShowBackface(GL_FALSE);

    // load map
    grid_ = new Grid("res/megoworld/default.map");
    addChild("grid", grid_);
    // camera
    player_ = new MegoPlayer(grid_);
    game::cameraSet(player_->eye(), GL_TRUE);
    addChild("player", player_);

    // light
    glm::ivec3 size = grid_->size();
    GLint lightArea = size.x * size.y / NUM_LIGHT;
    GLint lightLen = ceil(sqrt(lightArea));
    lightLen = lightLen <= 0 ? 1 : lightLen;
    GLint nLightX = size.x / lightLen;
    nLightX = nLightX <= 1 ? 2 : nLightX;
    lights_ = new game::Entity();
    for (int i = 0; i < NUM_LIGHT; ++i) {
        int x = lightLen * (i % nLightX);
        int y = lightLen * (i / nLightX);
        // light
        light_[i] = new game::Light(glm::mat3(glm::vec3(2.f / NUM_LIGHT),
                                              glm::vec3(1.f / NUM_LIGHT),
                                              glm::vec3(1.f / NUM_LIGHT)));
        game::lights.push_back(light_[i]);
        // mesh
        light_entity_[i] = new game::MeshEntity("res/lamp.obj",
                                                "res/lamp.mtl");
        light_entity_[i]->addChild("light", light_[i]);
        light_entity_[i]->scale(glm::vec3(.01f, .01f, .01f));
        light_entity_[i]->translate
            (grid_->indexToWorld(glm::ivec3(x + i * 4, y - i, size.z + 64)));
        // add
        std::stringstream ss;
        ss << "light" << i;
        lights_->addChild(ss.str(), light_entity_[i]);
    }
    addChild("all-lights", lights_);
}

MegoWorld::~MegoWorld() {
}
