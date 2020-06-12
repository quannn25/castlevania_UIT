#include "Bat.h"


Bat::Bat(float x1, float y1, int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BAT_ANI_SET_ID);
	SetAnimationSet(ani_set);

	type = eType::BAT;

	this->x = x1;
	this->y = y1;
	this->nx = nx1;
	this->health = 1;
	this->isIdle = true;
	this->deltaY = y1;

	vy = BAT_SPEED_Y;
	vx = BAT_SPEED_X * nx;
}

Bat::~Bat()
{
}

void Bat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BAT_FRAMEWIDTH;
	bottom = y + BAT_FRAMEHEIGHT;

}

void Bat::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* listObject)
{
	if (simon->GetY() >= y + 32 && simon->GetY() <= y + 96 && isIdle == true) // simon có độ cao thấp hơn Bat và trong 1 khoảng nhất định
	{
		if (abs(simon->GetX() - x) <= 150) // cách Bat 150_x
		{
			if (simon->GetX() >= x)
			{
				this->nx = 1;
				vx = BAT_SPEED_Y * nx;
			}
			else
			{
				this->nx = -1;
				vx = BAT_SPEED_Y * nx;
			}

			isIdle = false;
		}
	}

	if (isIdle)
	{
		return;
	}

	if (y - deltaY >= DeltaY)
	{
		vy = 0;
	}


	CGameObject::Update(dt); // Update dt, dx, dy

	y += dy;
	x += dx;

}

void Bat::Render()
{
	if (health <= 0)
		return;

	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	if(isIdle)
		animation_set->at(0)->Render(x, y, 255, isLeft);
	else
		animation_set->at(1)->Render(x, y, 255, isLeft);

	RenderBoundingBox();
}