#ifndef GRID_H_
#define GRID_H_

#include "../../game/Game.h"

class Grid : public game::Entity {

    GLint ***grid_map_;
    glm::mat4 ***grid_cell_trans_;
    glm::ivec3 spawn_point_, size_;
    GLfloat size_sqrd_;

    game::Mesh *brick_face_[6];
    game::Mesh *brick_face_detail_;
    glm::ivec3 detail_center_;

    static const GLint NUM_CELL_COLORS = 8;
    game::Material *brick_mtl_[NUM_CELL_COLORS];

    int timer_;
    glm::vec3 target_;

public:

    static const glm::vec3 DIR[], CRNR[];

    static const GLint CELL_BAD;
    static const GLint CELL_EMPTY;
    static const GLint CELL_TARGET;
    static const GLint MAX_TIME;

    static const glm::vec3 SZ;
    static const glm::vec3 SZI;

    static const glm::mat4 SCALE, SCALEI;

    static const GLfloat GRAVITY;
    static const GLint INTERACT_RD, INTERACT_RD_2;
    static const GLint DETAIL_RD, DETAIL_RD_2;

    Grid(std::string file);
	virtual ~Grid();

    void render();

    void drawCell(int x, int y, int z) {
        glm::ivec3 cell(x, y, z);
        if (localCell(cell) <= 0)
            return;
        // TODO optimize to put visible faces in some data-structure and update
        // when needed (no processing)
        for (int j = 0; j < 6; j++)
            if (localCell(cell + glm::ivec3(DIR[j])) <= 0){
                game::mtlSet(brick_mtl_[grid_map_[z][y][x]]);
                brick_face_[j]->render(grid_cell_trans_[z][y][x]);
            }
    }

    void drawCellDetail(int x, int y, int z, GLfloat r2) {
        game::setUniformBlendColor
            (glm::vec4(1, 1, 1, 2 - r2 / DETAIL_RD_2 * 2),
             glm::vec4(0, 0, 0, 1));
        glm::ivec3 cell(x, y, z);
        if (localCell(cell) <= 0)
            return;
        for (int j = 0; j < 6; j++)
            if (localCell(cell + glm::ivec3(0, 0, 1)) <= 0){
                game::mtlSet(brick_mtl_[grid_map_[z][y][x]]);
                brick_face_detail_->render(grid_cell_trans_[z][y][x]);
            }
    }

    GLint **operator[](int z) { return grid_map_[z]; }
	glm::ivec3 size() { return size_; }
	glm::ivec3 spawn_point() { return spawn_point_; }
	void set_detail_center(glm::ivec3 p) { detail_center_ = p; }

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
        for (int z = -INTERACT_RD * 2; z <= INTERACT_RD * 2; z++)
            for (int y = -INTERACT_RD; y <= INTERACT_RD; y++)
                for (int x = -INTERACT_RD; x <= INTERACT_RD; x++) {
                    glm::ivec3 r = glm::ivec3(x, y, z);
                    if (glm::length2(glm::ivec3(x, y, z / 2)) <= INTERACT_RD_2
                        && localCell(pLocal + r) > 0)
                            out.push_back(pLocal + r);
                }
        return out;
    }

    // hit

    GLint worldCellHit(glm::ivec3 cell) {
        GLint out = grid_map_[cell.z][cell.y][cell.x];
        if (localCell(cell) > 0)
            grid_map_[cell.z][cell.y][cell.x] = CELL_EMPTY;
        return out;
    }

    void localCellPlace(glm::ivec3 c, GLint v) {
        grid_map_[c.z][c.y][c.x] = v;
    }

    glm::ivec3 localCellPlace(glm::ivec3 cell, glm::vec3 worldInterPnt,
                              GLint v) {
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
            localCellPlace(targetCell, v);
        else
            targetCell = glm::ivec3(-1, -1, -1);
        return targetCell;
    }

    glm::vec3 getTarget(){
    	return target_;
    }

    // Timer

    int getTimer(){
    	return timer_;
    }

    void updateTimer(){
    	timer_--;
    }
};

#endif
