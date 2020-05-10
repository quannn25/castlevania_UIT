#include "SmallHeart.h"

SmallHeart::SmallHeart(float x1, float y1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SMALLHEART_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::SMALLHEART;

	this->x = x1;
	this->y = y1;
	this->xBackup = x;

	TimeDisplayMax = SMALLHEART_TIMEDISPLAYMAX;
	TimeDisplayed = 0;
	TimeWaited = 0;
	TimeWaitMax = SMALLHEART_TIMEWAITMAX;


	vy = SMALLHEART_GRAVITY;
	vx = SMALLHEART_SPEED_X;
}

void SmallHeart::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SMALLHEART_FRAMEWIDTH;
	bottom = y + SMALLHEART_FRAMEHEIGHT;
}

void SmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
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

	// vượt quá biên độ trục x cho phép thì đổi chiều vx
	if (x - xBackup >= DeltaX)
	{
		vx = -SMALLHEART_SPEED_X;
	}
	else
		if (x - xBackup <= -DeltaX)
		{
			vx = SMALLHEART_SPEED_X;
		}


	Item::Update(dt); // Update dt, dx, dy


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
		x += dx;
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
			vx = 0; // dừng dao động
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}

SmallHeart::~SmallHeart()
{
}

