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
    game::cameraSet(player_);
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

void MegoWorld::update() {
    GLfloat speed = game::key_down_[' '] * 3 + 2;

    // light
    if (game::key_down_['c'])
        light_entity_[0]->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_entity_[0]->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // camera
    if (game::key_down_['a'])
        player_->translate(-speed * player_->u() * .05f);
    if (game::key_down_['d'])
        player_->translate(+speed * player_->u() * .05f);
    if (game::key_down_['w'])
        player_->translate(-speed * player_->n() * .05f);
    if (game::key_down_['s'])
        player_->translate(+speed * player_->n() * .05f);
    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    player_->transform(player_->fpsRotation(speed * delta, GL_FALSE));
}
