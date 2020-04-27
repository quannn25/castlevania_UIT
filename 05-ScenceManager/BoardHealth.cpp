#include "BoardHealth.h"



BoardHealth::BoardHealth()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOARDHEALTH_ANI_SET_ID);

	SetAnimationSet(ani_set);
}


BoardHealth::~BoardHealth()
{
}

void BoardHealth::Draw(float x, float y, int PlayerHealth, int EnemyHealth)
{
	int i, j;

	for (i = 0; i < PlayerHealth; i++)
	{
		animation_set->at(FRAME_PLAYER)->Render(x + i * BOARDHEALTH_FRAMEWIDTH, y);
	}

	
	for (j = i; j < BOARDHEALTH_MAX_CELL; j++)
	{
		animation_set->at(FRAME_NONE)->Render(x + j * BOARDHEALTH_FRAMEWIDTH, y);
	}

	
	for (i = 0; i < EnemyHealth; i++)
	{
		animation_set->at(FRAME_ENEMY)->Render(x + i * BOARDHEALTH_FRAMEWIDTH, y + 18);
	}

	
	for (j = i; j < BOARDHEALTH_MAX_CELL; j++)
	{
		animation_set->at(FRAME_NONE)->Render(x + j * BOARDHEALTH_FRAMEWIDTH, y + 18);
	}

}