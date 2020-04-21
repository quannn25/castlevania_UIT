#ifndef BOARD_H
#define BOARD_H

#include "Sprites.h"
#include "Textures.h"
#include "Camera.h"

class Board
{
private:
	CSprite * BoardSprite;
	int x, y;

public:
	Board(int X, int Y);
	void Render();
	~Board();
};
#endif