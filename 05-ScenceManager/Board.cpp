#include "Board.h"

Board::Board(float x1, float y1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOARD_ANI_SET_ID);

	SetAnimationSet(ani_set);

	x = x1;
	y = y1;
}

void Board::Render()
{
	float x1 = Camera::GetInstance()->Getx();
	float y1 = Camera::GetInstance()->Gety();

	animation_set->at(0)->Render(x1, y1);
	font.Draw(x1, y1, "HAHAHA BA");
}


Board::~Board()
{
}