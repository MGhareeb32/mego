//#include "GirdGen.h"
//
//namespace GirdGen {
//
//static const GLint DIR[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
//
//void generate(glm::ivec3 size, int random, int smooth) {
//    GLint h[size.y][size.x];
//    // initial fill
//    for (int i = 0; i < size.y; ++i) {
//        for (int j = 0; j < size.x; ++j) {
//            h[i][j] = size.x / 2;
//        }
//    }
//    // earthquake
//    for (int i = 0; i < random; ++i) {
//        int x = rand() % size.x;
//        int y = rand() % size.y;
//        int z = rand() % size.z;
//        h[y][x] = z;
//        // smooth
//        for (int j = 0; j < smooth; ++j) {
//            for (int k = 0; k < 4; ++k) {
//                int newX = x + j * DIR[k][0];
//                int newY = y + j * DIR[k][1];
//                if (newX >= 0 && newX < size.x && newY >= 0 && newY < size.y)
//                    int newZ = (h[y][x] + h[newY][newX]) / 2;
//            }
//        }
//    }
//    // fill 3d array
//    GLint c[size.z][size.y][size.x];
//    for (int i = 0; i < size.y; ++i)
//        for (int j = 0; j < size.x; ++j)
//            for (int k = 0; k < h[i][j]; ++k)
//                c[size.z - k - 1][i][j] = 1;
//    // print
//        for (int z = 0; z < size.z; ++z, std::cout << std::endl)
//            for (int y = 0; y < size.y; ++y, std::cout << std::endl)
//                for (int x = 0; x < size.x; ++x)
//                    std::cout >> c[z][y][x] << " ";
//}
//
//}
