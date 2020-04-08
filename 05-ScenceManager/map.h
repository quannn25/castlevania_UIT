﻿#ifndef __MAP_H__
#define __MAP_H__


#include <stdio.h>

#include "Textures.h"
#include "Sprites.h"
#include "define.h"

#include "camera.h"

#include <fstream>
#include <iostream>


#define TILE_FRAME_WIDTH 64
#define TILE_FRAME_HEIGHT 64

class Map
{
private:
	CTextures * TileTexture;
	CSprite *TileSprite;

	int ScreenColumn;
	int ScreenRow;

	int RowMatrix; // số lượng dòng của ma trận tilemap txt
	int ColumnMatrix;// số lượng cột của ma trận tilemap txt

	int ColTile; // số lượng cột tile trong texture
	int RowTile;// số lượng dòng tile trong texture
	int CountTileFrame; // tổng số lương tile trong texture 
	int TileMap[500][500];

	int row; // dòng hiện tại camera đang đứng
	int column; //cột hiện tại camera đang đứng
	int x, y;


public:
	Map();
	~Map();
	void LoadMap();
	void ReadMapTXT(char * filename);
	void DrawMap(Camera * camera);

};




#endif