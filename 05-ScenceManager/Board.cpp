#include "Board.h"

Board::Board(int x1, int y1)
{
	BoardSprite = CSprites::GetInstance()->Get(ID_SPRITE_BOARDGAME);
	x = x1;
	y = y1;
}

void Board::Render()
{
	float x1 = Camera::GetInstance()->Getx();
	float y1 = Camera::GetInstance()->Gety();
	BoardSprite->Draw(x1, y1);
	font.Draw(x1, y1, "hahaha");
}


Board::~Board()
{
}