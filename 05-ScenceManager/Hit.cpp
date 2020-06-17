#include "Hit.h"

Hit::Hit(float x1, float y1)
{
	this->x = x1;
	this->y = y1;
	
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(HITEFFECT_ANI_SET_ID);
	SetAnimationSet(ani_set);

	isFinish = false;
	animation_set->at(0)->setCurrentFrame(-1);
}

Hit::~Hit()
{
}

void Hit::Update(DWORD dt)
{
	if (animation_set->at(0)->getCurrentFrame() == 1) // nếu là frame cuối thì xong
		isFinish = true;
}