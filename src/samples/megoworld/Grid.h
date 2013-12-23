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
    static const GLint INTERACT_RD;

    Grid(std::string file);
	virtual ~Grid();

    void render();

    void drawCell(int x, int y, int z) {
        if (grid_map_[z][y][x] <= 0)
            return;
        // TODO optimize as to loop on the sphere only
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

    std::vector<glm::ivec3> worldCloseCells(glm::vec3 p) {
        // TODO optimize as to loop on the sphere only
        std::vector<glm::ivec3> out;
        glm::ivec3 pLocal = worldToIndex(p);
        for (int z = -INTERACT_RD; z <= INTERACT_RD; z++)
            for (int y = -INTERACT_RD; y <= INTERACT_RD; y++)
                for (int x = -INTERACT_RD; x <= INTERACT_RD; x++) {
                    glm::ivec3 r = glm::ivec3(x, y, z);
                    if (glm::length(r) <= INTERACT_RD
                        && localCell(pLocal + r) > 0)
                            out.push_back(pLocal + r);
                }
        return out;
    }

    // hit

    void worldCellHit(glm::ivec3 cell) {
        if (localCell(cell) > 0)
            grid_map_[cell.z][cell.y][cell.x] = CELL_EMPTY;
    }

    void localCellPlace(glm::ivec3 cell, glm::vec3 worldInterPnt, GLint v) {
        // a vector that goes from center of brick to intersection point
        glm::vec3 interVector = SZI * (worldInterPnt - indexToWorld(cell)
                                       - .5f * glm::vec3(1.f, 1.f, -1.f) * SZ);

        // get closest normal
        glm::vec3 interNormal = DIR[0];
        for (int i = 1; i < 6; ++i)
            if (glm::dot(DIR[i], interVector)
                    > glm::dot(interNormal, interVector))
                interNormal = DIR[i];

        glm::ivec3 targetCell = cell + glm::ivec3(interNormal);
        if (localCell(targetCell) == CELL_EMPTY)
            grid_map_[targetCell.z][targetCell.y][targetCell.x] = v;
    }
};

#endif
