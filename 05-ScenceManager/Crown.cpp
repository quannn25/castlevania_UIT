#include "Crown.h"



Crown::Crown(float x, float y)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(CROWN_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::CROWN;
	this->x = x;
	this->y = y;
	this->y_backup = y;

	vy = -CROWN_GRAVITY;
	TimeDisplayMax = CROWN_TIMEDISPLAYMAX; // set time hiển thị tối đa
	TimeDisplayed = 0;
	TimeWaited = CROWN_TIMEWAITMAX;
	TimeWaitMax = CROWN_TIMEWAITMAX;
}

void Crown::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + CROWN_FRAMEWIDTH;
	bottom = y + CROWN_FRAMEHEIGHT;
}

void Crown::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	TimeDisplayed += dt;
	if (TimeDisplayed >= TimeDisplayMax)
	{
		isFinish = true;
		return;
	}
	Item::Update(dt);

	y += dy;

	if (y < y_backup - 32)
	{
		y = y_backup - 32;
	}
	
}

void Crown::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

Crown::~Crown()
{
}
