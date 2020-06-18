#include "StopWatch.h"

StopWatch::StopWatch()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(STOPWATCHITEM_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::STOPWATCH;
	isFinish = true;
}



StopWatch::~StopWatch()
{
}

void StopWatch::Create(float simonX, float simonY, int simonNx)
{
	ActiveTime = 0;
	isFinish = false;

}

void StopWatch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinish)
		return;
	ActiveTime += dt;
	if (ActiveTime >= 5000) // hoạt động trong 5s
	{
		ActiveTime = 0;
		isFinish = true;
	}
}

void StopWatch::Render()
{
}

bool StopWatch::isCollision(LPGAMEOBJECT obj)
{
	return false;
}

void StopWatch::RenderIcon(float x1, float y1)
{
	animation_set->at(0)->Render(x1, y1);
}

void StopWatch::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = top = right = bottom = 0;
}

void StopWatch::ReSetAniSetSwitchScene()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(STOPWATCHITEM_ANI_SET_ID);

	SetAnimationSet(ani_set);
}