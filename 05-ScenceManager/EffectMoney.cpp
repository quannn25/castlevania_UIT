#include "EffectMoney.h"



EffectMoney::EffectMoney(float X, float Y, eType typeEffectMoney)
{
	int type_aniset = 0;
	switch (typeEffectMoney)
	{
	case EFFECT_MONEY_100:
	{
		type_aniset = EFFECT100_ANI_SET_ID;
	}
		break;
	case EFFECT_MONEY_1000:
	{
		type_aniset = EFFECT1000_ANI_SET_ID;
	}
		break;
	default:
	{
		DebugOut(L"Effect not found!\n");
		type_aniset = EFFECT100_ANI_SET_ID;
	}
		break;
	}

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(type_aniset);
	SetAnimationSet(ani_set);

	this->x = X + EFFECTMONEY_FITX;
	this->y = Y - EFFECTMONEY_FITY;

	isFinish = false;
	animation_set->at(0)->setCurrentFrame(-1);
}

void EffectMoney::Update(DWORD dt)
{
	Effect::Update(dt);
	if (animation_set->at(0)->getCurrentFrame() == 1) // nếu là frame cuối thì xong, frame cuối trống
		isFinish = true;
}

EffectMoney::~EffectMoney()
{
}