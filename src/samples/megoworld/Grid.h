#ifndef GRID_H_
#define GRID_H_

#include "../../game/Game.h"

class Grid : public game::Entity {

    GLint ***grid_map_;
    glm::ivec3 spawn_point_, size_;
    GLfloat size_sqrd_;

    game::Mesh *brick_mesh_[6];

    static const GLint NUM_CELL_COLORS = 2;
    game::Material *brick_mtl_[NUM_CELL_COLORS];

public:

    static const glm::vec3 DIR[], CRNR[];

    static const GLint CELL_BAD;
    static const GLint CELL_EMPTY;
    static const glm::vec3 SZ;
    static const glm::vec3 SZI;

    static const glm::mat4 SCALE, SCALEI;

    static const GLfloat GRAVITY;

    Grid(std::string file);
	virtual ~Grid();

    void render();

    void drawCell(int x, int y, int z) {
        if (grid_map_[z][y][x] <= 0)
            return;
        glm::ivec3 cell(x, y, z);
        for (int j = 0; j < 6; j++)
            if (localCell(cell + glm::ivec3(DIR[j])) <= 0){
                game::mtlSet(brick_mtl_[grid_map_[z][y][x]]);
                brick_mesh_[j]->render(glm::translate(SCALE, glm::vec3(cell)));
            }
    }

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
