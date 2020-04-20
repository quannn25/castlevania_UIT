﻿#include "UpgradeMorningStar.h"



UpgradeMorningStar::UpgradeMorningStar()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(UPGRADEMORNINGSTAR_ANI_SET_ID);

	SetAnimationSet(ani_set);
	type = eID::UPGRADEMORNINGSTAR;
}

UpgradeMorningStar::UpgradeMorningStar(float X, float Y) : UpgradeMorningStar()
{
	this->x = X;
	this->y = Y;
	vy = UPGRADEMORNINGSTAR_GRAVITY;
	TimeDisplayMax = UPGRADEMORNINGSTAR_TIMEDISPLAYMAX; // set time hiển thị tối đa

}

void UpgradeMorningStar::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + UPGRADEMORNINGSTAR_FRAMEWEIGHT;
	bottom = y + UPGRADEMORNINGSTAR_FRAMEHEIGHT;
}

void UpgradeMorningStar::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
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

void UpgradeMorningStar::SetReward()
{
	
}


UpgradeMorningStar::~UpgradeMorningStar()
{
}
