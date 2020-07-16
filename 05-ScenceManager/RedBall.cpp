#include "RedBall.h"



RedBall::RedBall(float x, float y)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(REDBALL_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::REDBALL;
	this->x = 1420;
	this->y = 100;
	vy = REDBALL_GRAVITY;
	TimeDisplayMax = REDBALL_TIMEDISPLAYMAX; // set time hiển thị tối đa
	TimeDisplayed = 0;
	TimeWaited = 0;
	TimeWaitMax = REDBALL_TIMEWAITMAX;
}

void RedBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + REDBALL_FRAMEWIDTH;
	bottom = y + REDBALL_FRAMEHEIGHT;
}

void RedBall::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	if (TimeWaited < TimeWaitMax) // chưa xuất hiện thì out
	{
		TimeWaited += dt;
		return;
	}

	//TimeDisplayed += dt;
	//if (TimeDisplayed >= TimeDisplayMax)
	//{
	//	isFinish = true;
	//	return;
	//}
	Item::Update(dt);

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < listObject->size(); i++)
	{
		if (dynamic_cast<CBrick *>(listObject->at(i)))
			listObject_Brick.push_back(listObject->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm

														  // No collision occured, proceed normally
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

RedBall::~RedBall()
{
}
