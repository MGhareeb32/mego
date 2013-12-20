#include "CubeWorld.h"
#include "../../game/Grid.h"
using namespace std;

CubeWorld::CubeWorld() {
	game::sceneColorSet(glm::vec3(0.2f, 0.2f, 0.2f));
    game::fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), .1f);
    cam_ = new game::Camera();
    game::cameraSet(cam_);
    cam_->persp();
    game::mouseLock(GL_TRUE);
    // axes
    scale(glm::vec3(.5f, .5f, .5f));
    // obj
    std::ifstream input("res/cubeworld/map.in");
    int xlen,ylen,zlen;
    input >> xlen >> ylen >> zlen;
    cout << " size " << xlen << " " << ylen << " " << zlen << endl;
    Grid::setGridSize(xlen, ylen, zlen);
    // initialize map with -1
    for(int i =0;i < 100;i++)
    	for(int j =0 ;j  < 100;j++)
    		for(int k =0 ;k < 100;k++)
    			Grid::editGridMap(i,j,k,0);
    int nextBlock = 0;
    for (int z = 0; z < zlen; z++) {
        for (int x = 0; x < xlen; x++)
            for (int y = 0; y < ylen; y++) {
                int val;
            	input >> val;
            	Grid::editGridMap(x,y,z,val);
                // TODO add target
                if (val) {
                    game::MeshEntity *newEntity
                        = new game::MeshEntity("res/cubeworld/box.obj",
                                               "res/cubeworld/box.mtl",
                                               "res/cubeworld/box.png");
                    string name = "newEntity_";
                    name.append(1, (nextBlock++) + '0');
                    addChild(name, newEntity);
                    newEntity->scale(glm::vec3(.1f, .1f, .1f));
                    newEntity->translate(glm::vec3(x * Grid::getGridDelta(), y * Grid::getGridDelta(), z * Grid::getGridDelta()));
                    map_entities_.push_back(newEntity);
                }
            }
    }
    for (size_t i = 0; i < map_entities_.size(); i++)
        cout << map_entities_[i]->o() << endl;
    // light
    light_ = new game::Light(glm::vec3(1));
    addChild("light", light_);
    game::lights.push_back(light_);
    //
    light_entity_ = new game::MeshEntity("res/cubeworld/gem.obj",
                                         "res/cubeworld/gem.mtl",
                                         "res/cubeworld/gem.png");
    light_entity_->scale(glm::vec3(.05f, .05f, .05f));
    light_->addChild("light", light_entity_);
    light_->translate(glm::vec3(1.f, .5f, 0.f));
}

CubeWorld::~CubeWorld() {
}

void CubeWorld::update() {
    GLfloat speed = game::key_down_[' '] * 3 + 2;
    // light
    if (game::key_down_['c'])
        light_entity_->rotate(+speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    if (game::key_down_['v'])
        light_entity_->rotate(-speed, glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    // camera
    game::Camera* myCamera = game::cameraGet();
    if (game::key_down_['q'])
        myCamera->rotate(+speed, myCamera->n());
    if (game::key_down_['e'])
        myCamera->rotate(-speed, myCamera->n());

    if (game::key_down_['a'])
        myCamera->movePlayer(-speed * myCamera->u() * .05f);
    if (game::key_down_['d'])
        myCamera->movePlayer(+speed * myCamera->u() * .05f);
    if (game::key_down_['w'])
        myCamera->movePlayer(-speed * myCamera->n() * .05f);
    if (game::key_down_['s'])
        myCamera->movePlayer(+speed * myCamera->n() * .05f);

    if(game::key_down_['j'])
        myCamera->jump();

    // Gravity

    myCamera->moveDown(speed);

    // fps controls
    glm::vec2 delta = game::mouse_pos_ - game::mouse_pos_prev_;
    // TODO check if transform use translate
    myCamera->transform(myCamera->fpsRotation(speed * delta, GL_FALSE));
}
