#include "Item.h"



Item::Item()
{
	TimeDisplayed = 0;
	TimeWaitMax = 0;
	TimeDisplayMax = 0;
	TimeWaited = 0;
}


Item::~Item()
{
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void Item::Render()
{
	if (TimeWaited < TimeWaitMax)
	{
		return;
	}

	bool isLeft;
	if (nx > 0)
		isLeft = false;
	else
		isLeft = true;

	animation_set->at(0)->Render(x, y, 255, isLeft);
	RenderBoundingBox();
}

bool Item::GetFinish()
{
	return isFinish;
}

void Item::SetFinish(bool f)
{
	isFinish = f;
}

bool Item::isWaitingDisplay()
{
	return TimeWaited < TimeWaitMax;
}