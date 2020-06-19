#include "BoomerangItem.h"



BoomerangItem::BoomerangItem(float X, float Y)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANGITEM_ANI_SET_ID);
	SetAnimationSet(ani_set);

	x = X;
	y = Y;

	type = eType::BOOMERANGITEM;
	vy = BOOMERANGITEM_GRAVITY;
	TimeDisplayMax = BOOMERANGITEM_TIMEDISPLAYMAX; // set time hiển thị tối đa
	TimeDisplayed = 0;
	TimeWaited = 0;
	TimeWaitMax = BOOMERANGITEM_TIMEWAITMAX;


}


void BoomerangItem::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANGITEM_FRAMEWIDTH;
	bottom = y + BOOMERANGITEM_FRAMEHEIGHT;
}

void BoomerangItem::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	if (TimeWaited < TimeWaitMax)
	{
		TimeWaited += dt;
		return;
	}

	TimeDisplayed += dt;
	if (TimeDisplayed >= TimeDisplayMax)
	{
		isFinish = true;
		return;
	}

	Item::Update(dt); // Update dt, dx, dy


	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < listObject->size(); i++)
	{
		if (dynamic_cast<CBrick*>(listObject->at(i)))
			listObject_Brick.push_back(listObject->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm



	if (coEvents.size() == 0)
	{
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		y += min_ty * dy + ny * 0.4f;

		if (ny != 0)
		{
			vy = 0;
		}
	}


	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}


BoomerangItem::~BoomerangItem()
{
}