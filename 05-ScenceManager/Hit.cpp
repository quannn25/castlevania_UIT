#include "Hit.h"

Hit::Hit(int x1, int y1)
{
	this->x = x1;
	this->y = y1;
	
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HITEFFECT_ANI_SET_ID);

	SetAnimationSet(ani_set);
}

Hit::~Hit()
{
}

void Hit::Update()
{
	if (animation_set->at(0)->getCurrentFrame() == 1) // nếu là frame cuối thì xong, frame cuối trống
		isFinish = true;
}