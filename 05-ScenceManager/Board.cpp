#include "Board.h"

Board::Board(float x1, float y1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOARD_ANI_SET_ID);

	SetAnimationSet(ani_set);

	x = x1;
	y = y1;
}

void Board::Render(Simon *simon , int state, Weapon * subWeapon, int RemainingTime)
{
	float x1 = Camera::GetInstance()->Getx();
	float y1 = Camera::GetInstance()->Gety();

	animation_set->at(0)->Render(x1, y1);

	font.Draw(x1 + 110, y1 + 15, FillNumber(std::to_string(simon->GetScore()), 6)); // SCORE
	font.Draw(x1 + 400 + 15, y1 + 15 + 18, FillNumber(std::to_string(simon->GetHeartCollected()), 2)); // HEART
	font.Draw(x1 + 400 + 60, y1 + 14, FillNumber(std::to_string(state), 2)); // STATE
	font.Draw(x1 + 400 + 15, y1 + 15 + 18 + 18, FillNumber(std::to_string(simon->GetLive()), 2)); // LIVE
	font.Draw(x1 + 290, y1 + 15, FillNumber(std::to_string(RemainingTime), 4)); //TIME

	boardHealth.Draw(x1 + 110, y1 + 30, simon->GetHealth(), 5);

	if (subWeapon != NULL)
	{
		if (subWeapon->GetType() == eType::DAGGER)
		{
			subWeapon->RenderIcon(x1 + 310, y1 + 40);
		}

	}
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