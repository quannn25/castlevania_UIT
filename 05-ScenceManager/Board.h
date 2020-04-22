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
	float x, y;
	string information;
public:
	Board(float x, float y);
	void Render();
	~Board();
};
#endif