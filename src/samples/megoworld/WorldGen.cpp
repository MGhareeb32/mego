#include "WorldGen.h"

namespace WorldGen {

static const GLint DIR[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                 {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
static glm::ivec3 size_;

int color(int x, int y, int z) {
    if (rand() % 100 == 1)
        return 1;
    if (rand() % 100 == 2)
        return 5;
    if (rand() % 100 == 3)
        return 6;
    if (z + (rand() % (size_.z / 6)) >= size_.z * 2 / 3)
        return 7;
    if (z - (rand() % (size_.z / 12)) <= size_.z / 3)
        return 3;
    return (x * y
            + (rand() % (size_.x * size_.y / 8))
            - (size_.x * size_.y) / 16) > size_.x * size_.y / 4 ? 4 : 2;
}

void generate(glm::ivec3 size, int seed, int random, int smooth) {
    size_ = size;
    GLint h[size.y][size.x];
    memset(h, 0, sizeof h);
    // earthquake
    for (int i = 0; i < random; ++i) {
        int x = rand() % size.x;
        int y = rand() % size.y;
        int z = rand() % size.z;
        h[y][x] = z;
        // smooth
        for (int k = 0; k < 8; ++k) {
            int newX = x + DIR[k][0];
            int newY = y + DIR[k][1];
            if (newX >= 0 && newX < size.x && newY >= 0 && newY < size.y)
                h[newY][newX] = (h[y][x] + h[newY][newX]) / 2 + 1;
        }
    }
    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
            for (int k = 0; k < 4; ++k) {
                int newX = x + DIR[k][0];
                int newY = y + DIR[k][1];
                if (newX >= 0 && newX < size.x && newY >= 0 && newY < size.y)
                    h[newY][newX] = (h[y][x] + h[newY][newX]) / 2;
            }
    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
            for (int k = 4; k < 8; ++k) {
                int newX = x + DIR[k][0];
                int newY = y + DIR[k][1];
                if (newX >= 0 && newX < size.x && newY >= 0 && newY < size.y)
                    h[newY][newX] = (h[y][x] + h[newY][newX]) / 2;
            }
    // print
    std::ofstream out("res/megoworld/gen.map");
    out << size.x << " " << size.y << " " << size.z << std::endl;
    out << (size.x / 2) << " " << (size.y / 2) << " " << size.z << std::endl;
    for (int z = 0; z < size.z; ++z, out << std::endl)
        for (int y = 0; y < size.y; ++y, out << std::endl)
            for (int x = 0; x < size.x; ++x)
                out << (z < h[y][x] ? color(x, y, z) : 0) << " ";
    out.close();
}

}
