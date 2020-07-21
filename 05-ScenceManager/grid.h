#ifndef GRID_H
#define	GRID_H

#include <vector>
#include <stdlib.h>
#include <iomanip>
#include "Game.h" 
#include "Sprites.h"
#include "Map.h"
#include "Brick.h"
#include "define.h"


using namespace std;

#define GRID_CELL_WIDTH 256 //256
#define GRID_CELL_HEIGHT 224 //224

#define GRID_MAX_ROW 10
#define GRID_MAX_COL 10


class Grid
{
private:
	vector<LPGAMEOBJECT> cells[GRID_MAX_ROW][GRID_MAX_COL];
public:
	void GetListObject(vector<LPGAMEOBJECT>& ListObj);

	void ResetOnCam(vector<LPGAMEOBJECT>& ListObj);

	void Insert(LPGAMEOBJECT obj, int row, int col); //Thêm object vào grid

	void BigBBox(LPGAMEOBJECT obj); //Thêm object vào grid

	Grid();
	~Grid();
};

#endif 