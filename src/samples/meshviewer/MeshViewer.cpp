#include "MeshViewer.h"

MeshViewer::MeshViewer() {
    set_mesh((game::Mesh*)game::ResMgr::load("res/mesh/lego.obj"));
    set_mtl((game::Material*)game::ResMgr::load("res/mesh/lego.mtl"));
    game::Camera *cam = new game::Camera();
    game::cameraSet(cam);
    scale(glm::vec3(.4f, .4f, .4f));
    game::lightGet()->translate(glm::vec3(2, 0, 0));
}

MeshViewer::~MeshViewer() {
}

void MeshViewer::update() {
    rotate(1.f, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    // camera
    game::Camera* myCamera = game::cameraGet();
    bool speed = game::key_down_[' '];
    if (game::key_down_['w'])
        myCamera->rotate(+(speed * 4 + 1), myCamera->u());
    if (game::key_down_['s'])
        myCamera->rotate(-(speed * 4 + 1), myCamera->u());

    if (game::key_down_['q'])
        myCamera->rotate(+(speed * 4 + 1), myCamera->n());
    if (game::key_down_['e'])
        myCamera->rotate(-(speed * 4 + 1), myCamera->n());

    if (game::key_down_['d'])
        myCamera->rotate(+(speed * 4 + 1), glm::vec3(0, 0, 1));
    if (game::key_down_['a'])
        myCamera->rotate(-(speed * 4 + 1), glm::vec3(0, 0, 1));

    if (game::key_down_['A'])
        myCamera->translate(-myCamera->u() * .1f);
    if (game::key_down_['D'])
        myCamera->translate(myCamera->u() * .1f);
}

void MeshViewer::render() {
    game::setUniformBlendColor(glm::vec4(.5f, 0.f, .5f, 1.f),
                               glm::vec4(.5f, .5f, .5f, .5f));
    MeshEntity::render();
}