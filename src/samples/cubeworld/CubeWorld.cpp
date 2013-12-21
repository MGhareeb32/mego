#include "CubeWorld.h"
using namespace std;

CubeWorld::CubeWorld() {
    game::sceneColorSet(glm::vec3(0.2f, 0.2f, 0.2f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    // axes
    scale(glm::vec3(.5f, .5f, .5f));
    // obj
    std::ifstream input("res/cubeworld/map.in");
    int xlength, ylength, zlength;
    input >> xlength >> ylength >> zlength;
    int nextBlock = 0;
    for (int z = 0; z < zlength; z++) {
        for (int x = 0; x < xlength; x++)
            for (int y = 0; y < ylength; y++) {
                int exist;
                input >> exist;
                if (exist) {
                    game::MeshEntity *newEntity
                        = new game::MeshEntity("res/cubeworld/triangle.obj",
                                               "res/cubeworld/box.mtl",
                                               "res/cubeworld/box.png");
                    string name = "newEntity_";
                    name.append(1, (nextBlock++) + '0');
                    addChild(name, newEntity);
//                    newEntity->scale(glm::vec3(.5f, .5f, .5f));
                    newEntity->scale(glm::vec3(.1f, .1f, .1f));
                    newEntity->translate(glm::vec3((x - xlength / 2) * .2f, (y - ylength / 2) * .2f, (z - zlength / 2) * .2f));
                    map_entities_.push_back(newEntity);
                }
            }
    }
    // light
    light_ = new game::Light(glm::vec3(.8f));
    addChild("light", light_);
    game::lights.push_back(light_);
    //
    light_entity_ = new game::MeshEntity("res/lamp.obj",
                                         "res/lamp.mtl");
    light_entity_->scale(glm::vec3(.05f, .05f, .05f));
    light_->addChild("light", light_entity_);
    light_->translate(glm::vec3(1.f, .5f, .25f));
}

CubeWorld::~CubeWorld() {
}

void CubeWorld::update() {
    GLfloat speed = 2 - game::key_down_[' '] * 1.5f;
    // light
    if (game::key_down_['c'])
        light_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
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
    if (game::key_down_['r'])
        myCamera->translate(-speed * myCamera->v() * .05f);
    if (game::key_down_['f'])
        myCamera->translate(+speed * myCamera->v() * .05f);
    if (game::key_down_['w'])
        myCamera->translate(-speed * myCamera->n() * .05f);
    if (game::key_down_['s'])
        myCamera->translate(+speed * myCamera->n() * .05f);
    // camera modes
    if (game::key_down_['0']) {
        myCamera->~Camera();
        myCamera = new game::Camera();
        game::cameraSet(myCamera);
    }
    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    myCamera->transform(myCamera->fpsRotation(speed * delta, GL_FALSE));
    // intersect
    for (std::size_t i = 0; i < map_entities_.size(); ++i)
        if (map_entities_[i]->intersects(myCamera->o(), myCamera->n()))
            map_entities_[i]->mtl()->set_ka(glm::vec3(.5f, 0, 0));
        else
            map_entities_[i]->mtl()->set_ka(glm::vec3(.1f, .1f, .1f));
}
