#include "Grid.h"

const glm::vec3 Grid::DIR[6] = {
    glm::vec3(+1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0, +1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, 0, +1),
    glm::vec3(0, 0, -1), };

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
    glm::vec3(0, -1, -1), };

const GLint Grid::CELL_BAD = -1;
const GLint Grid::CELL_EMPTY = 0;
const GLint Grid::CELL_TARGET = NUM_CELL_COLORS - 1;
const GLint Grid::MAX_TIME = 5000;

const glm::vec3 Grid::SZ = glm::vec3(0.1f, 0.1f, 0.05f);
const glm::vec3 Grid::SZI
    = glm::vec3(1 / Grid::SZ.x, 1 / Grid::SZ.y, 1 / Grid::SZ.z);

const glm::mat4 Grid::SCALE = glm::scale(SZ);
const glm::mat4 Grid::SCALEI = glm::scale(SZI);

const GLfloat Grid::GRAVITY = -SZ.z * .04f;

const GLint Grid::INTERACT_RD = 4;
const GLint Grid::INTERACT_RD_2 = Grid::INTERACT_RD * INTERACT_RD;
const GLint Grid::DETAIL_RD = 10;
const GLint Grid::DETAIL_RD_2 = Grid::DETAIL_RD * DETAIL_RD;

Grid::Grid(std::string file) {
    std::ifstream input(file.c_str());
    // size
    input >> size_.x >> size_.y >> size_.z;
    input >> spawn_point_.x >> spawn_point_.y >> spawn_point_.z;
    size_sqrd_ = glm::dot(size_, size_);
    // cells
    target_ = glm::vec3(-1, -1, -1);
    grid_map_ = new GLint**[(GLint) size_.z];
    for (int z = 0; z < size_.z; z++) {
        grid_map_[z] = new GLint*[(GLint) size_.y];
        for (int y = 0; y < size_.y; y++) {
            grid_map_[z][y] = new GLint[(GLint) size_.x];
            for (int x = 0; x < size_.x; x++) {
                input >> grid_map_[z][y][x];
                if (grid_map_[z][y][x] == Grid::CELL_TARGET)
                    target_ = indexToWorld(glm::ivec3(x, y, z));
            }
        }
    }
    input.close();
    std::cout << "map loaded" << std::endl;
    // cells transformations
    grid_cell_trans_ = new glm::mat4**[(GLint) size_.z];
    for (int z = 0; z < size_.z; z++) {
        grid_cell_trans_[z] = new glm::mat4*[(GLint) size_.y];
        for (int y = 0; y < size_.y; y++) {
            grid_cell_trans_[z][y] = new glm::mat4[(GLint) size_.x];
            for (int x = 0; x < size_.x; x++) {
                glm::ivec3 cell(x, y, z);
                grid_cell_trans_[z][y][x] = glm::translate(SCALE,
                        glm::vec3(cell));
            }
        }
    }
    // prepare box model
    std::stringstream ss;
    for (int i = 0; i < 6; ++i) {
        ss.str("");
        ss << "res/megoworld/mego-brick-face-" << i << ".obj";
        brick_face_[i] = (game::Mesh *)game::ResMgr::load(ss.str());
    }
    brick_face_detail_ = (game::Mesh *)game::ResMgr::load
        ("res/megoworld/mego-brick-face-detail.obj");
    // prepare materials
    for (int i = 1; i < NUM_CELL_COLORS; ++i) {
        ss.str("");
        ss << "res/megoworld/mego-brick-mat-" << i << ".mtl";
        brick_mtl_[i] = (game::Material *) game::ResMgr::load(ss.str());
        brick_mtl_[i]->set_texture(
            (game::Texture *) game::ResMgr::load
                ("res/megoworld/mego-brick-tex.png"));
    }
    // initialize timer
    timer_ = target_.x < 0 ? -1 : Grid::MAX_TIME;
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
    // TODO optimize as to render on same material bricks at the same time
    // TODO optimize as to render visible faces only
    // draw faces
    for (int z = 0; z < size_.z; z++)
        for (int y = 0; y < size_.y; y++)
            for (int x = 0; x < size_.x; x++)
                drawCell(x, y, z);
    // add detail
    GLfloat r2;
    for (int z = -DETAIL_RD; z <= DETAIL_RD; z++)
        for (int y = -DETAIL_RD; y <= DETAIL_RD; y++)
            for (int x = -DETAIL_RD; x <= DETAIL_RD; x++) {
                if ((r2 = glm::length2(glm::vec3(x, y, z))) <= DETAIL_RD_2)
                    drawCellDetail(x + detail_center_.x,
                                   y + detail_center_.y,
                                   z + detail_center_.z, r2);
            }
}
