#include "Weapon.h"



Weapon::Weapon()
{
	isFinish = 1;
}


Weapon::~Weapon()
{
	SAFE_DELETE(animation_set);
}

int Weapon::GetNx()
{
	return nx;
}

void Weapon::SetNx(int nx1)
{
	this->nx = nx1;
}

void Weapon::Update()
{

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
}

void Weapon::SetPosition(float X, float Y)
{
	this->x = X;
	this->y = Y;
}

void Weapon::UpdatePositionFitSimon()
{
}

int Weapon::GetFinish()
{
	return isFinish;
}

void Weapon::SetFinish(bool f)
{
	isFinish = f;
}

eID Weapon::GetType()
{
	return TypeWeapon;
}
