#include "Weapon.h"



Weapon::Weapon()
{
	isFinish = 1;
}


Weapon::~Weapon()
{
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
	RenderBoundingBox();
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
	return Type;
}

void Weapon::RenderBoundingBox()
{

	RECT rect;
	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 92);

}

Item * Weapon::GetNewItem(int id, eID type, float x, float y)
{
	if (type == eID::TORCH)
	{
		if (id == 1 || id == 4)
			return new LargeHeart(x, y);
		if (id == 100 || id == 101)
			return new UpgradeMorningStar(x, y);

		return new LargeHeart(x, y);
	}
}