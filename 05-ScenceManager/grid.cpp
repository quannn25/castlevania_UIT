#include "Grid.h"

Grid::Grid(vector<LPGAMEOBJECT> ListObj)
{
	LoadGrid(ListObj);
}

Grid::~Grid()
{

}

void Grid::LoadGrid(vector<LPGAMEOBJECT> ListObj)// list tong playScence, tru Simon
{
	//id x, y ,w,h
	float l, t, r, b;
	for (size_t i = 1; i < ListObj.size(); i++)
	{
		ListObj[i]->GetBoundingBox(l, t, r, b);
		Insert(ListObj[i], l, t, r, b);
	}
}

void Grid::GetListObject(vector<LPGAMEOBJECT>& ListObj, Camera * camera)
{
	ListObj.clear(); // clear list
	//ResetTake(); // goi o noi dung luon

	int rowBottom = floor((camera->Gety() + camera->GetScreenHeight()) / (float)GRID_CELL_HEIGHT);
	int rowTop = floor((camera->Gety()) / (float)GRID_CELL_HEIGHT);

	int colLeft = floor((camera->Getx()) / (float)GRID_CELL_WIDTH);// fun -5??
	int colRight = floor((camera->Getx() + camera->GetScreenWidth()) / (float)GRID_CELL_WIDTH);


	for (int row = rowTop; row <= rowBottom; row++)
		for (int col = colLeft; col <= colRight; col++)
		{
			for (int i = 0; i < cells[row + GRID_BASE][col + GRID_BASE].size(); i++)
			{
				if (cells[row + GRID_BASE][col + GRID_BASE].at(i)->GetHealth() > 0)
				{
					if (cells[row + GRID_BASE][col + GRID_BASE].at(i)->onCam == false)
					{
						ListObj.push_back(cells[row + GRID_BASE][col + GRID_BASE].at(i));
						cells[row + GRID_BASE][col + GRID_BASE].at(i)->onCam = true;
					}
				}
			}
		}
}




void Grid::ResetTake(vector<LPGAMEOBJECT>& ListObj) // list obj bao gom simon, but ko duyet qua simon
{
	for (int i = 1; i < ListObj.size(); i++) // i = 0 = Simon
	{
		ListObj[i]->onCam = false;
	}
}

void Grid::Insert(LPGAMEOBJECT dtObj, float x, float y, float w, float h)
{
	int Top = floor(y / (float)GRID_CELL_HEIGHT);
	int Bottom = floor((y + h) / (float)GRID_CELL_HEIGHT);

	int Left = floor(x / (float)GRID_CELL_WIDTH);
	int Right = floor((x + w) / (float)GRID_CELL_WIDTH);

	dtObj->onCam = false;

	for (int row = Top; row <= Bottom; row++)
	{
		for (int col = Left; col <= Right; col++)
		{
			cells[row + GRID_BASE][col + GRID_BASE].push_back(dtObj);
		}
	}

}
