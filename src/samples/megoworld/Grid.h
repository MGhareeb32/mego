#ifndef GRID_H_
#define GRID_H_

#include "../../game/Game.h"

static const GLint NUM_CELL_COLORS = 2;
static const GLint CELL_EMPTY = 0;

static const GLfloat CELL_SIZE = 0.1f;
static const GLfloat CELL_SIZE_INV = 1 / CELL_SIZE;
static const GLfloat CELL_HEIGHT = CELL_SIZE / 2;
static const GLfloat CELL_HEIGHT_INV = 1 / CELL_HEIGHT;

class Grid : public game::Entity {

    GLint ***grid_map_;
    glm::ivec3 spawn_point_, size_;

    game::Mesh *brick_mesh_;
    game::Material *brick_mtl_[NUM_CELL_COLORS];
public:

    Grid(std::string file);
	virtual ~Grid();

	GLint **operator[](int z) { return grid_map_[z]; }
	glm::ivec3 size() { return size_; }
	glm::ivec3 spawn_point() { return spawn_point_; }

	glm::ivec3 worldToIndex(glm::vec3 world) {
        return glm::ivec3(glm::vec3(CELL_SIZE_INV * glm::vec4(world, 1)));
    }

	glm::vec3 indexToWorld(glm::ivec3 index) {
	    return glm::vec3(CELL_SIZE * glm::vec4(index, 1));
    }

	void render();
};

#endif
