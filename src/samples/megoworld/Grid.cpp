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

const GLfloat Grid::SZ = 0.1f;
const GLfloat Grid::SZI = 1 / Grid::SZ;

const GLfloat Grid::GRAVITY = -SZ * .04f;

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
    brick_mesh_ = (game::Mesh *)game::ResMgr::load("res/megoworld/mego-brick-cube.obj");
    // prepare materials
    for (int i = 1; i < NUM_CELL_COLORS; ++i) {
        std::stringstream ss;
        ss << "res/megoworld/mego-brick-" << i << ".mtl";
        brick_mtl_[i] = (game::Material *)game::ResMgr::load(ss.str());
        brick_mtl_[i]->set_texture((game::Texture *)game::ResMgr::load("res/cubeworld/box.png"));
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
    glm::mat4 scale = glm::scale(SZ, SZ, SZ);
    int cell;
    for (int z = 0; z < size_.z; z++)
        for (int y = 0; y < size_.y; y++)
            for (int x = 0; x < size_.x; x++)
                if (grid_map_[z][y][x] > 0) {
                    cell = grid_map_[z][y][x];
                    game::mtlSet(brick_mtl_[cell]);
                    brick_mesh_->render(glm::translate(scale * glm::mat4(1), glm::vec3(x, y, z)));
                }
}
