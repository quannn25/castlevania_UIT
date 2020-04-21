#include "Board.h"

Board::Board(int x1, int y1)
{
	BoardSprite = CSprites::GetInstance()->Get(ID_SPRITE_BOARDGAME);
	x = x1;
	y = y1;
}

void Board::Render()
{
	BoardSprite->Draw(x, y);
}


Board::~Board()
{
}