#include "RubixCube.h"

RubixCube::RubixCube() {
    rubix = (game::Mesh*)game::ResMgr::load("res/rubix/rubix-face.obj");
    flat = (game::Mesh*)game::ResMgr::load("res/rubix/flat-face.obj");
    // initialize faces
    game::Camera *cam = new game::Camera();
    cam->persp();
    game::cameraSet(cam);
    // faces
    for (GLfloat x = -1.f; x <= +1.f; x += 1.f)
        for (GLfloat y = -1.f; y <= +1.f; y += 1.f)
            for (GLfloat z = -1.f; z <= +1.f; z += 1.f)
                if (x != y || y != z || z != 0.f)
                    addChild(std::string("cube") + char(childrenCount() + '0'),
                             new SmallCube(glm::vec3(x, y, z), flat, rubix));
    scale(glm::vec3(.4f, .4f, .4f));
    game::fogSet(glm::vec4(0, 0, 0, 1), 0);
    //
    game::Light *l1 = new game::Light(glm::vec3(.5f));
    game::lights.push_back(l1);
    l1->translate(glm::vec3(10, 0, 10));
    //
    game::Light *l2 = new game::Light(glm::vec3(.5f));
    game::lights.push_back(l2);
    l2->translate(glm::vec3(5, 5, 10));
}

RubixCube::~RubixCube() {
    std::vector<std::string> ch = childrenNames();
    for (std::size_t i = 0; i < ch.size(); ++i) {
        game::Entity* c = getChild(ch[i]);
        removeChild(ch[i]);
        c->~Entity();
    }
    rubix->~Mesh();
    flat->~Mesh();
}

void RubixCube::update() {
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
    // select
    if (game::key_down_['0'])
        select(-1, -1);
    if (game::key_down_['1'])
        select(0, -1);
    if (game::key_down_['2'])
        select(0, 0);
    if (game::key_down_['3'])
        select(0, 1);
    if (game::key_down_['4'])
        select(1, -1);
    if (game::key_down_['5'])
        select(1, 0);
    if (game::key_down_['6'])
        select(1, 1);
    if (game::key_down_['7'])
        select(2, -1);
    if (game::key_down_['8'])
        select(2, 0);
    if (game::key_down_['9'])
        select(2, 1);
    // move
    if (game::key_down_['z'])
        spin(90);
    if (game::key_down_['x'])
        spin(-90);
    game::MeshEntity::update();
}

void RubixCube::select(int axis, GLfloat n) {
    std::vector<std::string> v = childrenNames();
    // de-select
    if (axis == -1) {
        for (std::size_t i = 0; i < v.size(); ++i)
            *((SmallCube*)getChild(v[i]))->selected() = false;
    }
    // select
    else {
        glm::vec3 axes[3] = { glm::vec3(1, 0, 0),
                              glm::vec3(0, 1, 0),
                              glm::vec3(0, 0, 1) };
        // check
        std::set<SmallCube*> good;
        bool can = true;
        for (std::size_t i = 0; i < v.size() && can; ++i) {
            SmallCube* ch = (SmallCube*)getChild(v[i]);
            glm::vec3 org = ch->o();
            can = abs(ch->spin_ang()) < EPS;
            if (n <= org[axis] + EPS && n >= org[axis] -EPS)
                good.insert(ch);
        }
        // select
        if (can)
            for (std::size_t i = 0; i < v.size(); ++i)
                if (good.find((SmallCube*)getChild(v[i])) != good.end())
                    *((SmallCube*)getChild(v[i]))->selected() = true,
                    ((SmallCube*)getChild(v[i]))->set_spin_axis(axes[axis]);
                else
                    *((SmallCube*)getChild(v[i]))->selected() = false;
    }
}

void RubixCube::spin(GLfloat ang) {
    // check
    std::vector<std::string> v = childrenNames();
    std::set<SmallCube*> good;
    bool can = true;
    for (std::size_t i = 0; i < v.size() && can; ++i) {
        SmallCube* ch = (SmallCube*)getChild(v[i]);
        can = abs(ch->spin_ang()) < EPS;
    }
    if (can)
        for (std::size_t i = 0; i < v.size(); ++i)
            if (*((SmallCube*)getChild(v[i]))->selected())
                ((SmallCube*)getChild(v[i]))->set_spin_ang(ang);
}
