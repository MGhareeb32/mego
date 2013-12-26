#include "game/Game.h"
#include "samples/rubix/RubixCube.h"
#include "samples/meshviewer/MeshViewer.h"
#include "samples/pointing/Pointing.h"
#include "samples/megoworld/MegoWorld.h"
#include "samples/megoworld/WorldGen.h"

const GLfloat DISPLAY_WIDTH = 800;
const GLfloat DISPLAY_HEIGHT = 600;
const GLfloat DISPLAY_FPS = 60;

int main(int argc, char **argv) {
    WorldGen::generate(glm::ivec3(256, 256, 64), 19920804, 256 * 256, 1);
    game::init(argc, argv, DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
    game::sceneSet(new MegoWorld());
    game::start(DISPLAY_FPS);
    return 0;
}
