#include <iostream>
#include <fstream>

#include "MeshViewer.h"
using namespace std;
using namespace game;

MeshViewer::MeshViewer() {
    game::sceneColorSet(glm::vec3(0.2f, 0.2f, 0.2f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    cam_->persp();
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_NONE);
    // room
    room_entity_ = new game::MeshEntity("res/viewer/room.obj",
                                        "res/viewer/room.mtl",
                                        "res/viewer/room.png");
    addChild("room", room_entity_);
    // obj
    obj_entity_ = new game::MeshEntity("res/viewer/batman.obj",
                                       "res/viewer/batman.mtl",
                                       "res/viewer/batman.png");
    addChild("obj", obj_entity_);
    obj_entity_->rotate(90, glm::vec3(1, 0, 0));
    obj_entity_->rotate(-45, glm::vec3(0, 0, 1));
    obj_entity_->scale(glm::vec3(.7f, .7f, .7f));
    // light
    for (int i = 0; i < N_LIGHT; ++i) {
        light_entity_[i] = new game::MeshEntity("res/viewer/gem.obj",
                                                "res/viewer/gem.mtl");
        const char *name[] = {"1", "2", "3", "4"};
        addChild(name[i], light_entity_[i] );
        light_[i] = new game::Light();
        game::lights.push_back(light_[i]);
        light_entity_[i]->addChild("light", light_[i]);
        light_entity_[i]->scale(glm::vec3(.05f, .05f, .05f));
        light_entity_[i]->translate(glm::vec3(.8f, .6f, .4f));
        light_entity_[i]->rotate(60 * i, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    }
}

MeshViewer::~MeshViewer() {
}

void MeshViewer::update() {
    GLfloat speed = game::key_down_[' '] * 3 + 2;
    // obj
    if (game::key_down_['b'])
        obj_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['n'])
        obj_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // light
    if (game::key_down_['c'])
        light_entity_[0]->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_entity_[0]->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    // camera
    game::Camera* myCamera = game::cameraGet();
    if (game::key_down_['q'])
        myCamera->rotate(+speed, myCamera->n());
    if (game::key_down_['e'])
        myCamera->rotate(-speed, myCamera->n());
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
    myCamera->transform(myCamera->fpsRotation(speed * delta, GL_FALSE));
}
