#include <iostream>
#include <fstream>

#include "MeshViewer.h"
using namespace std;
using namespace game;

MeshViewer::MeshViewer() {
    game::sceneColorSet(glm::vec3(0.1f, 0.1f, 0.1f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    cam_->persp(50, game::screen_size().x / game::screen_size().y);
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_NONE);
    // room
    room_entity_ = new game::MeshEntity("res/viewer/room.obj",
                                        "res/viewer/room.mtl",
                                        "res/viewer/room.png");
    room_entity_->scale(glm::vec3(2.f, 2.f, 2.f));
    addChild("room", room_entity_);

    // obj
    obj_entity_ = new game::MeshEntity("res/viewer/teapot.obj",
                                       "res/viewer/teapot.mtl");
    obj_entity_->rotate(-60, glm::vec3(0, 0, 1));
    obj_entity_->scale(glm::vec3(.5f, .5f, .5f));
    addChild("obj", obj_entity_);

    // light
    lights_ = new game::Entity();
    for (int i = 0; i < N_LIGHT; ++i) {
        std::stringstream ss;
        ss << "light" << i;
        // light
        game::Light *light_ = new game::Light(glm::vec3(1.5f / N_LIGHT));
        game::lights.push_back(light_);
        // mesh
        game::MeshEntity *light_entity_
            = new game::MeshEntity("res/lamp.obj", "res/lamp.mtl");
        light_entity_->addChild("light", light_);
        light_entity_->scale(glm::vec3(.05f, .05f, .05f));
        light_entity_->translate(glm::vec3(2.f, 0.f, (i % 3) - 1));
        light_entity_->rotate(360 / N_LIGHT * i, glm::vec3(0, 0, 1));
        std::cout << "H" << std::endl;
        lights_->addChild(ss.str(), light_entity_);
    }
    addChild("all-lights", lights_);
    scale(glm::vec3(.5f, .5f, .5f));
}

MeshViewer::~MeshViewer() {
}

void MeshViewer::update() {
    GLfloat speed = game::key_down_[' '] * 3 + 2;
    // obj
    if (game::key_down_['z'])
        lights_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['x'])
        lights_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // camera
    game::Camera* myCamera = game::cameraGet();
    if (game::key_down_['q'])
        obj_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['e'])
        obj_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['a'])
        myCamera->translate(-speed * myCamera->u() * .05f);
    if (game::key_down_['d'])
        myCamera->translate(+speed * myCamera->u() * .05f);
    if (game::key_down_['w'])
        myCamera->translate(-speed * myCamera->n() * .05f);
    if (game::key_down_['s'])
        myCamera->translate(+speed * myCamera->n() * .05f);
    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    myCamera->transform(myCamera->fpsRotation(speed * delta, GL_FALSE,
                                              GL_TRUE));
}
