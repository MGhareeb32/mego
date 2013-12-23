#ifndef GRID_H_
#define GRID_H_

#include "../../game/Game.h"

class Grid : public game::Entity {

    GLint ***grid_map_;
    glm::ivec3 spawn_point_, size_;
    GLfloat size_sqrd_;

    game::Mesh *brick_mesh_;

    static const GLint NUM_CELL_COLORS = 2;
    game::Material *brick_mtl_[NUM_CELL_COLORS];
public:

    static const glm::vec3 DIR[], CRNR[];

    static const GLint CELL_BAD;
    static const GLint CELL_EMPTY;

    static const GLfloat SZ;
    static const GLfloat SZI;

    static const GLfloat GRAVITY;

    Grid(std::string file);
	virtual ~Grid();

    void render();

    GLint **operator[](int z) { return grid_map_[z]; }
	glm::ivec3 size() { return size_; }
	glm::ivec3 spawn_point() { return spawn_point_; }

	// conversion

	glm::ivec3 worldToIndex(glm::vec3 world) {
        return glm::ivec3(SZI * world);
    }

	glm::vec3 indexToWorld(glm::ivec3 index) {
	    return SZ * glm::vec3(index);
    }

	// inside check

    GLboolean localIsInside(glm::ivec3 v) {
        return v.x >= 0 && v.x < size_.x
            && v.y >= 0 && v.y < size_.y
            && v.z >= 0 && v.z < size_.z;
    }

    GLboolean worldIsInside(glm::vec3 pos) {
        return localIsInside(worldToIndex(pos));
    }

    // get data

    int localCell(glm::ivec3 v) {
        return localIsInside(v) ? grid_map_[v.z][v.y][v.x] : CELL_BAD;
    }

    int worldCell(glm::vec3 pos) {
        return localCell(worldToIndex(pos));
    }

};

#endif
