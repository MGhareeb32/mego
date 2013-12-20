/*
 * Grid.h
 *
 *  Created on: Dec 20, 2013
 *      Author: Yasser
 */

#ifndef GRID_H_
#define GRID_H_

class Grid {
public:
	Grid();
	virtual ~Grid();
	static void editGridMap(int x, int y, int z, int val);
	static int getGridMapAt(int x, int y, int z);
	static void setGridSize(int x, int y, int z);
	static int getGridX();
	static int getGridY();
	static int getGridZ();
	static float getGridDelta();
};

#endif /* GRID_H_ */
