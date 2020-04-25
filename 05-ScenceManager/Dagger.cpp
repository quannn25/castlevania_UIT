#include "Dagger.h"



Dagger::Dagger()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(DAGGER_ANI_SET_ID);

	SetAnimationSet(ani_set);
	type = eID::DAGGER;
}


Dagger::~Dagger()
{
}

void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;


	x += dx;
}

void Dagger::Create(float simonX, float simonY, int simonNx)
{
	Weapon::Create(simonX, simonY + 10, simonNx);
	vx = DAGGER_SPEED * nx;
}

void Dagger::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + DAGGER_FRAMEWIDTH;
	bottom = y + DAGGER_FRAMEHEIGHT;
}

bool Dagger::isCollision(LPGAMEOBJECT obj)
{

	CGameObject *gameObj = dynamic_cast<CGameObject*>(obj);
	if (gameObj->GetHealth() <= 0)
		return false;

	RECT rect, rect1;
	float l, t, r, b;
	float l1, t1, r1, b1;

	GetBoundingBox(l, t, r, b);
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;

	gameObj->GetBoundingBox(l1, t1, r1, b1);
	rect1.left = l1;
	rect1.top = t1;
	rect1.right = r1;
	rect1.bottom = b1;
	return CGame::GetInstance()->AABBCheck(rect, rect1);
}

void Dagger::Render()
{
	if (x - Camera::GetInstance()->Getx() < 0 || x - Camera::GetInstance()->Getx() > Camera::GetInstance()->GetScreenWidth())
	{
		isFinish = true;
		return;
	}
	Weapon::Render();
}