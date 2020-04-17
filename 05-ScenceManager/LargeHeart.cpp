#include "LargeHeart.h"



LargeHeart::LargeHeart()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(LARGEHEART_ANI_SET_ID);

	SetAnimationSet(ani_set);

}

LargeHeart::LargeHeart(float X, float Y) : LargeHeart()
{
	this->x = X;
	this->y = Y;
	vy = LARGEHEART_GRAVITY;
	TimeDisplayMax = LARGEHEART_TIMEDISPLAYMAX;
}

void LargeHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + LARGEHEART_FRAMEWIDTH; //weight height define lai
	bottom = y + LARGEHEART_FRAMEHEIGHT;
}

void LargeHeart::Update(DWORD dt, CGameObject * listObject)
{
	dy = vy * dt;
}

LargeHeart::~LargeHeart()
{
}
