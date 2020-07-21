#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{

}


void Grid::GetListObject(vector<LPGAMEOBJECT>& ListObj)
{
	ListObj.clear(); // clear list

	int rowBottom = floor((Camera::GetInstance()->Gety() + Camera::GetInstance()->GetScreenHeight()) / (float)GRID_CELL_HEIGHT);
	int rowTop = floor((Camera::GetInstance()->Gety()) / (float)GRID_CELL_HEIGHT);

	int colLeft = floor((Camera::GetInstance()->Getx()) / (float)GRID_CELL_WIDTH);
	int colRight = floor((Camera::GetInstance()->Getx() + Camera::GetInstance()->GetScreenWidth()) / (float)GRID_CELL_WIDTH);

	//DebugOut(L"[Grid] left on Camera = %d\n", colLeft);
	//DebugOut(L"[Grid] right on Camera = %d\n", colRight);
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
void Grid::BigBBox(LPGAMEOBJECT obj)
{
	for (int i = 0; i < GRID_MAX_ROW; i++)
		for (int j = 0; j < GRID_MAX_COL; j++)
			cells[i][j].push_back(obj);

}

void Grid::ResetOnCam(vector<LPGAMEOBJECT>& ListObj) // list obj bao gom simon, but ko duyet qua simon
{
	for (UINT i = 1; i < ListObj.size(); i++) // i = 0 = Simon
	{
		ListObj[i]->SetOnCam(false);
	}
}

void Grid::Insert(LPGAMEOBJECT dtObj, int row, int col)
{
	/*int Top = floor(y / (float)GRID_CELL_HEIGHT);
	int Bottom = floor((y + h) / (float)GRID_CELL_HEIGHT);

	int Left = floor(x / (float)GRID_CELL_WIDTH);
	int Right = floor((x + w) / (float)GRID_CELL_WIDTH);*/

	//dtObj->SetOnCam(false);

	//for (int row = Top; row <= Bottom; row++) // 1 Obj có thể ở 2 hàng
	//{
	//	for (int col = Left; col <= Right; col++) // 1 Obj có thể ở 2 cột
	//	{
	//		cells[row][col].push_back(dtObj); // ở mỗi "ô" (hàng r cột l) thì có 1 vector Obj tương ứng
	//	}
	//}

	dtObj->SetOnCam(false);
	cells[row][col].push_back(dtObj);

}
