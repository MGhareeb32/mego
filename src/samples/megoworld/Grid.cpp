#include "Grid.h"

const glm::vec3 Grid::DIR[6] = {
    glm::vec3(+1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0, +1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 0, +1),
    glm::vec3(0, 0, -1),
};

const glm::vec3 Grid::CRNR[12] = {
    glm::vec3(+1, +1, 0),
    glm::vec3(+1, -1, 0),
    glm::vec3(-1, +1, 0),
    glm::vec3(-1, -1, 0),

    glm::vec3(+1, 0, +1),
    glm::vec3(+1, 0, -1),
    glm::vec3(-1, 0, +1),
    glm::vec3(-1, 0, -1),

    glm::vec3(0, +1, +1),
    glm::vec3(0, +1, -1),
    glm::vec3(0, -1, +1),
    glm::vec3(0, -1, -1),
};

const GLint Grid::CELL_BAD = -1;
const GLint Grid::CELL_EMPTY = 0;

const glm::vec3 Grid::SZ
    = glm::vec3(0.1f, 0.1f, 0.05f);
const glm::vec3 Grid::SZI
    = glm::vec3(1 / Grid::SZ.x, 1 / Grid::SZ.y, 1 / Grid::SZ.z);

const glm::mat4 Grid::SCALE = glm::scale(SZ);
const glm::mat4 Grid::SCALEI = glm::scale(SZI);

const GLfloat Grid::GRAVITY = -SZ.z * .04f;

Grid::Grid(std::string file) {
    std::ifstream input(file.c_str());
    // size
    input >> size_.x >> size_.y >> size_.z;
    input >> spawn_point_.x >> spawn_point_.y >> spawn_point_.z;
    size_sqrd_ = glm::dot(size_, size_);
    // cells
    grid_map_ = new GLint**[(GLint)size_.z];
    for (int z = 0; z < size_.z; z++) {
        grid_map_[z] = new GLint*[(GLint)size_.y];
        for (int y = 0; y < size_.y; y++) {
            grid_map_[z][y] = new GLint[(GLint)size_.x];
            for (int x = 0; x < size_.x; x++)
                input >> grid_map_[z][y][x];
        }
    }
    input.close();
    std::cout << "map loaded" << std::endl;
    // prepare box model
    for (int i = 0; i < 6; ++i)
        brick_mesh_[i] = (game::Mesh *)game::ResMgr::load
            (std::string("res/megoworld/mego-brick-cube-")
             + (char)(i + '0') + ".obj");
    // prepare materials
    for (int i = 1; i < NUM_CELL_COLORS; ++i) {
        std::stringstream ss;
        ss << "res/megoworld/mego-brick-" << i << ".mtl";
        brick_mtl_[i] = (game::Material *)game::ResMgr::load(ss.str());
        brick_mtl_[i]->set_texture
            ((game::Texture *)game::ResMgr::load("res/cubeworld/box.png"));
    }
}

Grid::~Grid() {
    for (int z = 0; z < size_.z; z++) {
        for (int y = 0; y < size_.y; y++)
            delete[] grid_map_[z][y];
        delete[] grid_map_[z];
    }
    delete[] grid_map_;
}

void Grid::render() {
    for (int z = 0; z < size_.z; z++)
        for (int y = 0; y < size_.y; y++)
            for (int x = 0; x < size_.x; x++)
                drawCell(x, y, z);
}
