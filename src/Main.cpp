#include "game/Game.h"
#include "../src/samples/rubix/RubixCube.h"
#include "../src/samples/meshviewer/MeshViewer.h"
#include "../src/samples/cubeworld/CubeWorld.h"
#include "../src/samples/pointing/Pointing.h"
#include "../src/samples/megoworld/MegoWorld.h"

const GLfloat DISPLAY_WIDTH = 1024;
const GLfloat DISPLAY_HEIGHT = 768;
const GLfloat DISPLAY_FPS = 30;

int main(int argc, char **argv) {
    game::init(argc, argv, DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
    game::sceneSet(new MegoWorld());
    game::start(DISPLAY_FPS);
    return 0;
}
