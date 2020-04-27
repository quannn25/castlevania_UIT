#include "Board.h"

Board::Board(float x1, float y1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOARD_ANI_SET_ID);

	SetAnimationSet(ani_set);

	x = x1;
	y = y1;
}

void Board::Render(Simon *simon , int state)
{
	float x1 = Camera::GetInstance()->Getx();
	float y1 = Camera::GetInstance()->Gety();

	animation_set->at(0)->Render(x1, y1);

	font.Draw(x1 + 110, y1 + 15, FillNumber(std::to_string(simon->GetScore()), 6));
	font.Draw(x1 + 400 + 15, y1 + 15 + 18, std::to_string(simon->GetHeartCollected()));
	font.Draw(x1 + 400 + 60, y1 + 15, std::to_string(state));
	font.Draw(x1 + 400 + 15, y1 + 15 + 18 + 18, std::to_string(simon->GetLive()));

	boardHealth.Draw(x1 + 110, y1 + 30, simon->GetHealth(), 5);
}


Board::~Board()
{
}

string Board::FillNumber(string s, int MaxNumber)
{
	while (s.size() < MaxNumber)
		s = "0" + s;
	return s;
}