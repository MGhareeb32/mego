#include "MeshViewer.h"

MeshViewer::MeshViewer() {
    game::sceneColorSet(glm::vec3(0.1f, 0.1f, 0.1f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    cam_->persp();
    game::mouseLock(GL_TRUE);
    glutSetCursor(GLUT_CURSOR_NONE);
    // load
    obj_mesh_ = (game::Mesh*)game::ResMgr::load
        ("res/mesh/viewer/teapot.obj");
    obj_mtl_ = (game::Material*)game::ResMgr::load
        ("res/mesh/viewer/teapot.mtl");
    axes_mesh_ = (game::Mesh*)game::ResMgr::load
        ("res/mesh/viewer/room.obj");
    axes_mtl_ = (game::Material*)game::ResMgr::load
        ("res/mesh/viewer/room.mtl");
    light_mesh_ = (game::Mesh*)game::ResMgr::load
        ("res/mesh/viewer/gem.obj");
    light_mtl_ = (game::Material*)game::ResMgr::load
        ("res/mesh/viewer/gem.mtl");
    // axes
    set_mesh(axes_mesh_);
    set_mtl(axes_mtl_);
    // obj
    obj_entity_ = new game::MeshEntity(obj_mesh_);
    obj_entity_->set_mtl(obj_mtl_);
    addChild("obj", obj_entity_);
    obj_entity_->scale(glm::vec3(.25f, .25f, .25f));
    // light
    light_ = new game::Light(glm::vec3(10));
    addChild("light", light_);
    game::lights.push_back(light_);
    //
    light_entity_= new game::MeshEntity(light_mesh_);
    light_entity_->set_mtl(light_mtl_);
    light_entity_->scale(glm::vec3(.05f, .05f, .05f));
    light_->addChild("light", light_entity_);
    light_->translate(glm::vec3(.8f, .6f, .4f));
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
    if (game::key_down_['x'])
        obj_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['z'])
        obj_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
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
    if (game::key_down_['w'])
        myCamera->translate(-speed * myCamera->n() * .05f);
    if (game::key_down_['s'])
        myCamera->translate(+speed * myCamera->n() * .05f);
    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    myCamera->transform(myCamera->fpsRotation(speed * delta, GL_FALSE));
}
