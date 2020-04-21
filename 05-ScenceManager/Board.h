#ifndef BOARD_H
#define BOARD_H

#include "Sprites.h"
#include "Textures.h"
#include "camera.h"
#include "Font.h"

class Board
{
private:
	CSprite * BoardSprite;
	Font font;
	int x, y;
	string information;
public:
	Board(int X, int Y);
	void Render();
	~Board();
};
#endif