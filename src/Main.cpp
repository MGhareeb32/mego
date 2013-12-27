#include "game/Game.h"
#include "samples/rubix/RubixCube.h"
#include "samples/meshviewer/MeshViewer.h"
#include "samples/pointing/Pointing.h"
#include "samples/megoworld/MegoWorld.h"
#include "samples/megoworld/WorldGen.h"

const GLfloat DISPLAY_WIDTH = 800;
const GLfloat DISPLAY_HEIGHT = 600;
const GLfloat DISPLAY_FPS = 30;

int main(int argc, char **argv) {
    game::init(argc, argv, DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
    game::sceneSet(new MegoWorld());
    game::start(DISPLAY_FPS);
    return 0;
}
