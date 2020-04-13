#ifndef GRID_H
#define	GRID_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Game.h" 
#include "Sprites.h" 
#include "Map.h"
#include "Brick.h"
#include "define.h"


using namespace std;

#define GRID_CELL_WIDTH 256
#define GRID_CELL_HEIGHT 224
#define GRID_BASE 10



class Grid
{

private:
	vector<LPGAMEOBJECT> listObjectGame;
	vector<LPGAMEOBJECT> cells[500][500];


public:
	void GetListObject(vector<LPGAMEOBJECT>& ListObj, Camera * camera);

	void ResetTake(vector<LPGAMEOBJECT>& ListObj);


	void Insert(LPGAMEOBJECT obj, float x, float y, float w, float h); //Thêm object vào grid


	void LoadGrid(vector<LPGAMEOBJECT> ListObj);

	Grid(vector<LPGAMEOBJECT> ListObj);
	~Grid();
};

#endif 