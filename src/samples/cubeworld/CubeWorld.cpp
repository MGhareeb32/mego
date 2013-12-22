#include "CubeWorld.h"

CubeWorld::CubeWorld() {
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	game::sceneColorSet(glm::vec3(0.2f, 0.2f, 0.2f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    // camera
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    reset();
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
                        = new game::MeshEntity("res/box.obj",
                                               "res/default.mtl",
                                               "res/box.png");
                    std::string name = "newEntity_";
                    name.append(1, (nextBlock++) + '0');
                    addChild(name, newEntity);
                    newEntity->scale(glm::vec3(SZ / 2, SZ / 2, SZ / 2));
                    newEntity->translate(glm::vec3(x * SZ, y * SZ, z * SZ));
                    map_entities_.push_back(newEntity);
                }
            }
    }
    // lights
    int nLightX = (xlength + 4) / 5;
    int nLightY = (ylength + 4) / 5;
    light_ = new game::Light*[nLightX];
    light_entity_ = new game::MeshEntity*[nLightX];
    for (int i = 0; i < nLightX * nLightY; ++i) {
        GLfloat x = 5 * SZ * ((i % nLightX) + .5f) - 2.5f * SZ;
        GLfloat y = 5 * SZ * ((i / nLightX) + .5f) - 2.5f * SZ;
        // light
        light_[i] = new game::Light(glm::vec3(1.f / nLightX));
        std::stringstream ss("light");
        ss << i;
        addChild(ss.str(), light_[i]);
        game::lights.push_back(light_[i]);
        // model
        light_entity_[i] = new game::MeshEntity("res/lamp.obj",
                                                "res/lamp.mtl");
        light_entity_[i]->scale(glm::vec3(.05f, .05f, .05f));
        light_[i]->addChild("light", light_entity_[i]);
        light_[i]->translate(glm::vec3(x, y, 2.f));
    }
    // center
    translate(glm::vec3(-xlength * SZ / 2, -ylength * SZ / 2, 0.f));
}

CubeWorld::~CubeWorld() {
}

void CubeWorld::reset() {
    cam_->lookAt(glm::vec3(0, 0, 5),
                 glm::vec3(0, 0, 0),
                 glm::vec3(0, 1, 1));
    cam_->persp();
}

void CubeWorld::update() {
    GLfloat speed = 2 - game::key_down_[' '] * 1.5f;

    // camera
    if (game::key_down_['w'])
        cam_->translate(-speed * cam_->n() * .05f);
    if (game::key_down_['s'])
        cam_->translate(+speed * cam_->n() * .05f);
    if (game::key_down_['a'])
        cam_->translate(-speed * cam_->u() * .05f);
    if (game::key_down_['d'])
        cam_->translate(+speed * cam_->u() * .05f);
    if (game::key_down_['q'])
        cam_->rotate(+speed, cam_->n());
    if (game::key_down_['e'])
        cam_->rotate(-speed, cam_->n());

    // fps
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    cam_->transform(cam_->fpsRotation(speed * delta, GL_FALSE));
}
