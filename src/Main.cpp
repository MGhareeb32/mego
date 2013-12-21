#include "game/Game.h"
#include "../src/samples/rubix/RubixCube.h"
#include "../src/samples/meshviewer/MeshViewer.h"
#include "../src/samples/cubeworld/CubeWorld.h"

const GLfloat DISPLAY_WIDTH = 640;
const GLfloat DISPLAY_HEIGHT = 640;
const GLfloat DISPLAY_FPS = 30;

glm::vec3 random_vec3_xy() {
    return glm::vec3(1.0 * rand() / RAND_MAX, 2.0 * rand() / RAND_MAX, 0);
}

glm::vec3 random_vec3_xyz() {
    return glm::vec3(1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX);
}

int main(int argc, char **argv) {
    glm::vec2 s = glm::vec2(0.27, -0.29);
    glm::vec2 ar[3] = { glm::vec2(1.41, 0.00), glm::vec2(-1.41, 0.00), glm::vec2(-0.00, 1.41) };
    std::cout << game::pointInPolygon(std::vector<glm::vec2>(ar, ar + 3), s);
//    exit(0);
//    srand(1);
//    std::cout << glm::sin(90) << std::endl;
//    int nIter = 0;
//    while (++nIter) {
//        // before rotation
//        glm::vec3 a[3] = { glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), random_vec3_xy() };
//        std::vector<glm::vec3> v(a, a + 3);
//        for (int i = 0; i < v.size(); ++i)
//            std::cout << v[i] << " ";
//        std::cout << std::endl;
//        glm::vec3 x = random_vec3_xy();
//        glm::vec3 y = glm::vec3(0, 0, 1);
//        bool ans1 = game::faceIntersects(v, x, y);
//        std::cout << ans1 << std::endl << std::endl;
//        // after rotation
//        glm::mat4 rot
//            = glm::rotate(glm::mat4(1), random_vec3_xy()[0] * 360, random_vec3_xyz());
//        for (int i = 0; i < v.size(); ++i)
//            v[i] = glm::vec3(rot * glm::vec4(v[i], 0));
//        for (int i = 0; i < v.size(); ++i)
//            std::cout << v[i] << " ";
//        std::cout << std::endl;
//        x = glm::vec3(rot * glm::vec4(x, 0));
//        y = glm::vec3(rot * glm::vec4(y, 0));
//        bool ans2 = game::faceIntersects(v, x, y);
//        std::cout << ans2 << std::endl << std::endl << std::endl;
//        if (ans1) {
//            std::cout << nIter << std::endl;
//            break;
//        }
//    }
    //exit(0);
    game::init(argc, argv, DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
    game::sceneSet(new CubeWorld());
    game::start(DISPLAY_FPS);
    return 0;
}
