#include "Item.h"



Item::Item()
{
	TimeDisplayed = 0;
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
	if (isFinish == true)
		return;

	animation_set->at(0)->Render(x, y, 255);
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