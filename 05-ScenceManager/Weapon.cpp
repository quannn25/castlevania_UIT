#include "Weapon.h"



Weapon::Weapon()
{
	isFinish = 1;
}


Weapon::~Weapon()
{
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void Weapon::Create(float simonX, float simonY, int simonNx)
{
	this->x = simonX;
	this->y = simonY;
	this->nx = simonNx;
	isFinish = 0;
}

void Weapon::Render()
{
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	animation_set->at(0)->Render(x, y, 255, isLeft); // at(0) chuỗi hành động đầu tiên của set (LEVEL 0)
	RenderBoundingBox();
}

void Weapon::UpdatePositionFitSimon()
{
}

bool Weapon::GetFinish()
{
	return isFinish;
}

void Weapon::SetFinish(bool f)
{
	isFinish = f;
}