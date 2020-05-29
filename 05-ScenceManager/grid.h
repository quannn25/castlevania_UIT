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

#define GRID_CELL_WIDTH 96 //256
#define GRID_CELL_HEIGHT 96 //224



class Grid
{
private:
	vector<LPGAMEOBJECT> cells[500][500];
public:
	void GetListObject(vector<LPGAMEOBJECT>& ListObj);

	void ResetOnCam(vector<LPGAMEOBJECT>& ListObj);

	void Insert(LPGAMEOBJECT obj, float x, float y, float w, float h); //Thêm object vào grid

	void LoadGrid(vector<LPGAMEOBJECT> ListObj);

	Grid(vector<LPGAMEOBJECT> ListObj);
	~Grid();
};

#endif 