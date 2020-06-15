#include "BrokenBrick.h"

BrokenBrick::BrokenBrick(int X, int Y, int type)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BROKENBRICK_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = X;
	this->y = Y;
	dx = dy = vx = vy = 0;
	typeBrick = type;
	switch (typeBrick)
	{
	case 1:
	{
		nx = -1;
		vx = nx * 0.15f;
		vy = -0.25f;
		break;
	}

	case 2:
	{
		nx = 1;
		vx = nx * 0.15f;
		vy = -0.2f;
		break;
	}

	case 3:
	{
		nx = -1;
		vx = nx * 0.07f;
		vy = -0.22f;
		break;
	}

	case 4:
	{
		nx = 1;
		vx = nx * 0.1f;
		vy = -0.27f;
		break;
	}
	}
}

void BrokenBrick::Update(DWORD dt)
{

	this->dt = dt;
	this->dx = vx * dt;
	this->dy = vy * dt;

	vy += BROKENBRICK_GRAVITY;

	x += dx;
	y += dy;

	float xCam = Camera::GetInstance()->Getx();
	float yCam = Camera::GetInstance()->Gety();
	float wid = Camera::GetInstance()->GetScreenWidth();
	float hei = Camera::GetInstance()->GetScreenHeight();

	if (x < xCam || x > xCam + wid || y > yCam + hei || y < yCam) // nếu là frame cuối thì xong
	{
		isFinish = true;
	}
}

void BrokenBrick::Render()
{
	animation_set->at(0)->setCurrentFrame(-1);

	Effect::Render();
}



BrokenBrick::~BrokenBrick()
{
}