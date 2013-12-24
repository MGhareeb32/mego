#include "Pointing.h"
using namespace std;

Pointing::Pointing() {
    game::sceneColorSet(glm::vec3(0.1f, 0.1f, 0.1f));
    game::fogSet(glm::vec4(0.15f, 0.15f, 0.15f, 1.f), .1f);
    cam_ = new game::Camera();
    cam_->lookAt(glm::vec3(.6f, .3f, .1f), glm::vec3(0, 0, 0),
                 glm::vec3(0, 0, 1));
    cam_->persp(50, game::screen_size().x / game::screen_size().y);
    game::cameraSet(cam_);
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    game::setUniformShowBackface(GL_TRUE);
    // obj
    std::ifstream input("res/pointing/map.in");
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
                        = new game::MeshEntity("res/pointing/batman.obj",
                                               "res/viewer/batman.mtl",
                                               "res/viewer/batman.png");
                    string name = "newEntity_";
                    name.append(1, (nextBlock++) + '0');
                    addChild(name, newEntity);
                    newEntity->scale(glm::vec3(.2f, .2f, .2f));
                    newEntity->rotate(-45, glm::vec3(0, 0, 1));
                    newEntity->translate(glm::vec3((x - xlength / 2) * .2f,
                                                   (y - ylength / 2) * .2f,
                                                   (z - zlength / 2) * .2f));
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
    light_->translate(10000.f * glm::vec3(1.f, -1.f, 1.f));
}

Pointing::~Pointing() {
}

void Pointing::update() {
    GLfloat speed = 2 - game::key_down_[' '] * 1.5f;

    // light
    if (game::key_down_['c'])
        light_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));

    // camera
    if (game::key_down_['q'])
        cam_->rotate(+speed, cam_->n());
    if (game::key_down_['e'])
        cam_->rotate(-speed, cam_->n());
    if (game::key_down_['a'])
        cam_->translate(-speed * cam_->u() * .05f);
    if (game::key_down_['d'])
        cam_->translate(+speed * cam_->u() * .05f);
    if (game::key_down_['r'])
        cam_->translate(-speed * cam_->v() * .05f);
    if (game::key_down_['f'])
        cam_->translate(+speed * cam_->v() * .05f);
    if (game::key_down_['w'])
        cam_->translate(-speed * cam_->n() * .05f);
    if (game::key_down_['s'])
        cam_->translate(+speed * cam_->n() * .05f);
    // camera modes
    if (game::key_down_['0']) {
        cam_->~Camera();
        cam_ = new game::Camera();
        game::cameraSet(cam_);
    }

    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    cam_->transform(cam_->fpsRotation(speed * delta, GL_FALSE, GL_TRUE));

    // pointing
    glm::vec3 point, nearestPoint;
    GLfloat minDist = numeric_limits<GLfloat>::infinity();
    game::MeshEntity *pointEntity = NULL;
    for (std::size_t i = 0; i < map_entities_.size(); ++i) {
        game::MeshEntity *entity = map_entities_[i];
        entity->mtl()->set_kd(glm::vec3(1, 1, 1));
        // intersects
        if (entity->getIntersect(cam_->o(), cam_->n())) {
            point = entity->getNearestIntersect(cam_->o(), cam_->n());
            GLfloat dist = glm::dot(cam_->o() - point, cam_->n());
            if (dist > 0 && dist < minDist) {
                minDist = dist;
                nearestPoint = point;
                pointEntity = entity;
            }
        }
    }
    if (pointEntity) {
        if (game::mouse_down_[GLUT_LEFT_BUTTON])
            pointEntity->rotate(5.f, glm::vec3(0, 0, 1), pointEntity->o());
        pointEntity->mtl()->set_kd(glm::vec3(1.f, 1.f, 3.f));
    }
}
