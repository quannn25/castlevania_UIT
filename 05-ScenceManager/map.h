#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "Textures.h"
#include "Sprites.h"
#include "define.h"
#include "camera.h"
#include "Simon.h"

class Map
{
private:
	CSprite *TileSprite;
	LPCWSTR texPath; // đọc file của titleTexture
	LPCWSTR txtPath; // file txt thuộc tính
	int frameWidth; // tính đc
	int frameHeight;// tính đc
	int index; // giá trị là phần tử của TileMap[500][500]
	int heightBoard; // độ cao boardGame

	int ScreenColumn; // screen_w / frame_w, tính ra đc
	int ScreenRow;// screen_w / frame_w, tính ra đc

	int RowMatrix; // số lượng dòng của ma trận map txt // doc file
	int ColumnMatrix;// số lượng cột của ma trận map txt // d f

	int ColTile; // số lượng cột tile trong texture // d f
	int RowTile;// số lượng dòng tile trong texture // d f
	int TileMap[500][500];

	int row; // dòng hiện tại camera đang đứng
	int column; //cột hiện tại camera đang đứng
	int x, y; // tọa độ vẽ

public:
	Map(LPCWSTR texpath, LPCWSTR txtPath);
	~Map();
	void load();
	void LoadMap();
	void ReadMapTXT(LPCWSTR filename);
	void DrawMap(Camera * camera);

	void SetTexPath(LPCWSTR path) { this->texPath = path; }
	void SetTxtPath(LPCWSTR path) { this->txtPath = path; }

	void loadFrameWidthHeight();

};
#endif