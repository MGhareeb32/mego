#include "game/Game.h"
#include "../src/samples/rubix/RubixCube.h"
#include "../src/samples/meshviewer/MeshViewer.h"
#include "../src/samples/pointing/Pointing.h"
#include "../src/samples/megoworld/MegoWorld.h"
//#include "../src/samples/megoworld/GirdGen.h"

const GLfloat DISPLAY_WIDTH = 800;
const GLfloat DISPLAY_HEIGHT = 600;
const GLfloat DISPLAY_FPS = 30;

int main(int argc, char **argv) {
//    GirdGen::generate(glm::ivec3(10, 10, 10), 0, 0);
    game::init(argc, argv, DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
    game::sceneSet(new MegoWorld());
    game::start(DISPLAY_FPS);
    return 0;
}
