#include "Map.h"


Map::Map(LPCWSTR texPath1, LPCWSTR txtPath1)
{
	texPath = texPath1;
	txtPath = txtPath1;
}


Map::~Map()
{
}
void Map::load()
{
	ReadMapTXT(txtPath);
	loadFrameWidthHeight();
	LoadMap();
}

void Map::loadFrameWidthHeight() // lấy đc frameWidth-Height
{
	D3DXIMAGE_INFO info;
	HRESULT result;


	result = D3DXGetImageInfoFromFile(texPath, &info); // đường dẫn file ảnh

	RECT s = { 0, 0, info.Width, info.Height };

	frameWidth = info.Width / ColTile;
	frameHeight = info.Height / RowTile;
	//DebugOut(L"frameWidth = %f\n", frameWidth);

}

void Map::LoadMap()
{
	// texture thi quan ly bang id luon
	// có nghĩa readMap txt đọc luôn id texture và các thông số fileIn >> RowMatrix >> ColumnMatrix >> ColTile >> RowTile >> CountTileFrame;
	
	// tạo 1 sprite tương ứng với id texture, time chuyển =0

	TileSprite = CSprites::GetInstance()->Get(ID_SPRITE_TILEMAP);// ID TEXTURE 30, ID SPRITE 50001

	MapWidth = frameWidth * ColumnMatrix; // chiều dài của map
	MapHeight = frameHeight * RowMatrix; //  chiều cao của map

	ScreenColumn = Window_Width / frameWidth;
	ScreenRow = Window_Height / frameHeight;
}

void Map::ReadMapTXT(LPCWSTR filename)
{
	ifstream fileIn;
	fileIn.open(filename, ios::in);

	if (fileIn)
	{
		fileIn >> RowMatrix >> ColumnMatrix >> ColTile >> RowTile >> heightBoard;
		for (int i = 0; i < RowMatrix; i++)
		{
			for (int j = 0; j < ColumnMatrix; j++)
			{
				fileIn >> TileMap[i][j];
			}
		}
		fileIn.close();
	}

}

void Map::DrawMap(Camera *camera)
{
	row = int(camera->Gety() / frameHeight);
	column = int(camera->Getx() / frameWidth); // vẽ ngay cột đang đứng để ko mất frame

	x = (float)(column * frameWidth); // x = n lần frameWidth, n = column
	y = (float)row * frameHeight;
	//DebugOut(L"x = %d\n", x);
	for (int i = 0; i < ScreenRow + 1; i++)
	{
		for (int j = 0; j < ScreenColumn + 1; j++)
		{
			if (!(row + i < 0 || row + i > RowMatrix || j + column < 0 || j + column > ColumnMatrix)) // cam đã kiểm tra chuyện này?
			{
				int index = TileMap[row + i][column + j];

				RECT r;
				r.left = (index % ColTile)*(frameWidth);
				r.top = (index / ColTile)*(frameHeight);
				r.right = r.left + frameWidth;
				r.bottom = r.top + frameHeight;
				
				TileSprite->DrawWithRect(r, x + frameWidth * j, y + frameHeight*i + heightBoard); // hàm vẽ thêm tham số index và tham số frameHeight, framewidth từ đây
				//DebugOut(L"x = %f\n", x + frameWidth * j);
			}

		}
	}

}
