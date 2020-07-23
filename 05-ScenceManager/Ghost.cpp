#include "Ghost.h"

Ghost::Ghost(int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GHOST_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->nx = nx1;
	health = 2;
	vx = GHOST_SPEED_X * this->nx;
	this->type = eType::GHOST;
	isAuto = false;
	isHurt = 0;
	hurtTime = 0;
}

Ghost::Ghost(float x1, float y1, int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GHOST_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = x1;
	this->y = y1;
	this->nx = nx1;
	health = 2;
	vx = GHOST_SPEED_X * this->nx;
	this->type = eType::GHOST;
	isAuto = false;
	isHurt = 0;
	hurtTime = 0;
}

Ghost::~Ghost()
{
}

void Ghost::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + GHOST_FRAMEWIDTH;
	bottom = y + GHOST_FRAMEHEIGHT;
}

void Ghost::Update(DWORD dt, Simon * simon, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHurt)
	{
		hurtTime += dt;
		if (hurtTime >= GHOST_HURT_TIME)
		{
			isHurt = false;
			hurtTime = 0;
		}
	}

	if (isAuto == true)
	{
		if (abs(this->x - simon->x) > 20)
			isAuto = false;
	}
	else
	{


		if (abs(this->x - simon->x) <= 5 && abs(this->y - (simon->y + 15)) <= 5)
		{

			isAuto = true;
			int t = rand() % 2;
			int p = rand() % 2;
			if (t == 0)
			{
				nx = 1;
				vx = nx * GHOST_SPEED_X;
				if (p == 0)
					vy = GHOST_SPEED_Y;
				else
					vy = -GHOST_SPEED_Y;
			}
			else
			{
				nx = -1;
				vx = nx * GHOST_SPEED_X;
				if (p == 0)
					vy = GHOST_SPEED_Y;
				else
					vy = -GHOST_SPEED_Y;
			}
		}
		else
		{
			if (this->x < simon->x)
			{
				this->nx = 1;
				vx = nx * GHOST_SPEED_X;
			}
			else
			{
				this->nx = -1;
				vx = nx * GHOST_SPEED_X;
			}

			if (this->y < simon->y + 15)
			{
				vy = GHOST_SPEED_Y;
			}
			else
			{
				vy = -GHOST_SPEED_Y;
			}
		}

		
	}




	CGameObject::Update(dt); // Update dt, dx, dy

	y += dy;
	x += dx;
}