/*
 * Grid.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: Yasser
 */

#include "Grid.h"

Grid::Grid() {
	// TODO Auto-generated constructor stub

}

// grid variables
char gridMap[100][100][100];
int gridX, gridY, gridZ;
const float GRID_DELTA = 0.2f;

// grid

void Grid::editGridMap(int x, int y, int z, int val) {
	gridMap[x][y][z] = val;
}

int Grid::getGridMapAt(int x, int y, int z) {
	return gridMap[x][y][z];
}

void Grid::setGridSize(int x, int y, int z) {
	gridX = x;
	gridY = y;
	gridZ = z;
}

int Grid::getGridX() {
	return gridX;
}
int Grid::getGridY() {
	return gridY;
}
int Grid::getGridZ() {
	return gridZ;
}

float Grid::getGridDelta() {
	return GRID_DELTA;
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}



