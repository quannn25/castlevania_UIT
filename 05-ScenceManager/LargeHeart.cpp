#include "LargeHeart.h"



LargeHeart::LargeHeart()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(LARGEHEART_ANI_SET_ID);
	SetAnimationSet(ani_set);
	type = eID::LARGEHEART;

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
	right = x + LARGEHEART_FRAMEWIDTH;
	bottom = y + LARGEHEART_FRAMEHEIGHT;
}

void LargeHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	TimeDisplayed += dt;
	if (TimeDisplayed >= TimeDisplayMax)
	{
		isFinish = true;
		return;
	}
	dy = vy * dt;

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (int i = 0; i < listObject->size(); i++)
	{
		if(dynamic_cast<CBrick *>(listObject->at(i)))
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx,rdy);

		y += min_ty * dy + ny * 0.4f;

		if (ny != 0)
		{
			vy = 0;
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void LargeHeart::SetReward()
{

	ItemManager::GetInstance()->HeartCollect += 5;
	DebugOut(L"[ITEM] +5 Heart. Tong = %d \n", ItemManager::GetInstance()->HeartCollect);
}

LargeHeart::~LargeHeart()
{
}
