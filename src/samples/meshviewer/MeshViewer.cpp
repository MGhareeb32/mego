#include <iostream>
#include <fstream>

#include "MeshViewer.h"
using namespace std;
using namespace game;

MeshViewer::MeshViewer() {
    game::fogSet(glm::vec4(0.4f, 0.4f, 0.4f, 1.f), 0.f);
    cam_ = new game::Camera();
    cam_->persp();
    game::cameraSet(cam_);
    // room
    room_entity_ = new game::MeshEntity("res/mesh/viewer/room.obj",
                                       "res/mesh/viewer/room.mtl");
    addChild("room", room_entity_);
    // obj
    obj_entity_ = new game::MeshEntity("res/mesh/viewer/teapot.obj",
                                       "res/mesh/viewer/teapot.mtl");
    addChild("obj", obj_entity_);
    obj_entity_->scale(glm::vec3(.4f, .4f, .4f));
    // light
    light_entity_ = new game::MeshEntity("res/mesh/viewer/gem.obj",
                                         "res/mesh/viewer/gem.mtl");
    addChild("light_mesh", light_entity_);
    light_ = new game::Light(glm::vec3(10));
    light_entity_->addChild("light", light_);
    light_entity_->scale(glm::vec3(.05f, .05f, .05f));
    light_entity_->translate(glm::vec3(1.f, .5f, .25f));
    game::lights.push_back(light_);
}

MeshViewer::~MeshViewer() {
}

void MeshViewer::update() {
    GLfloat speed = game::key_down_[' '] * 3 + 2;
    // obj
    if (game::key_down_['b'])
        light_->translate(+speed / 100 * glm::normalize(light_->o()));
    if (game::key_down_['n'])
        light_->translate(-speed / 100 * glm::normalize(light_->o()));
//    if (game::key_down_['x'])
//        obj_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
//    if (game::key_down_['z'])
//        obj_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // TODO map entities

    // light
    if (game::key_down_['c'])
        light_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    // camera
    game::Camera* myCamera = game::cameraGet();
    if (game::key_down_['w'])
        myCamera->rotate(+speed, myCamera->u());
    if (game::key_down_['s'])
        myCamera->rotate(-speed, myCamera->u());

    if (game::key_down_['q'])
        myCamera->rotate(+speed, myCamera->n());
    if (game::key_down_['e'])
        myCamera->rotate(-speed, myCamera->n());

    if (game::key_down_['d'])
        myCamera->rotate(+speed, glm::vec3(0, 0, 1));
    if (game::key_down_['a'])
        myCamera->rotate(-speed, glm::vec3(0, 0, 1));

    if (game::key_down_['A'])
        myCamera->translate(-myCamera->u() * .1f);
    if (game::key_down_['D'])
        myCamera->translate(myCamera->u() * .1f);
    // move camera
    if (game::key_down_['W'])
        myCamera->translate(-myCamera->n() * .1f);
    if (game::key_down_['S'])
        myCamera->translate(myCamera->n() * .1f);

}
