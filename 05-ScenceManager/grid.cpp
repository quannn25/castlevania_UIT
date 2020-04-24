#include "Grid.h"

Grid::Grid(vector<LPGAMEOBJECT> ListObj)
{
	LoadGrid(ListObj);
}

Grid::~Grid()
{

}

void Grid::LoadGrid(vector<LPGAMEOBJECT> ListObj)// list tổng playScence, trừ Simon
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

	int rowBottom = floor((camera->Gety() + camera->GetScreenHeight() -1) / (float)GRID_CELL_HEIGHT);
	int rowTop = floor((camera->Gety()) / (float)GRID_CELL_HEIGHT);

	int colLeft = floor((camera->Getx()) / (float)GRID_CELL_WIDTH);// fun -5??
	int colRight = floor((camera->Getx() + camera->GetScreenWidth() -1) / (float)GRID_CELL_WIDTH);


	for (int row = rowTop; row <= rowBottom; row++)
	{
		for (int col = colLeft; col <= colRight; col++)
		{
			for (UINT i = 0; i < cells[row][col].size(); i++)
			{
				if (cells[row][col].at(i)->GetHealth() > 0) // còn sống thì thêm vào
				{
					if (cells[row][col].at(i)->GetOnCam() == false) // nếu 1 obj có onCam rồi thì bỏ qua, vì 1 obj có thể nằm nhiều grid
					{ // nếu thêm obj 2 lần thì có thể nó bị đánh 2 lần ????
						ListObj.push_back(cells[row][col].at(i));
						cells[row][col].at(i)->SetOnCam(true);
					}
				}
			}
		}
	}
}




void Grid::ResetTake(vector<LPGAMEOBJECT>& ListObj) // list obj bao gom simon, but ko duyet qua simon
{
	for (UINT i = 1; i < ListObj.size(); i++) // i = 0 = Simon
	{
		ListObj[i]->SetOnCam(false);
	}
}

void Grid::Insert(LPGAMEOBJECT dtObj, float x, float y, float w, float h)
{
	int Top = floor(y / (float)GRID_CELL_HEIGHT);
	int Bottom = floor((y + h) / (float)GRID_CELL_HEIGHT);

	int Left = floor(x / (float)GRID_CELL_WIDTH);
	int Right = floor((x + w) / (float)GRID_CELL_WIDTH);

	dtObj->SetOnCam(false);

	for (int row = Top; row <= Bottom; row++) // 1 Obj có thể ở 2 hàng
	{
		for (int col = Left; col <= Right; col++) // 1 Obj có thể ở 2 cột
		{
			cells[row][col].push_back(dtObj); // ở mỗi "ô" (hàng r cột l) thì có 1 vector Obj tương ứng
		}
	}

}
