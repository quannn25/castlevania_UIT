#include "Zombie.h"

Zombie::Zombie(int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ZOMBIE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->nx = nx1;

	health = 1;

	vx = ZOMBIE_SPEED * this->nx;
}

Zombie::Zombie(float x1, float y1, int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ZOMBIE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->x = x1;
	this->y = y1;
	this->nx = nx1;

	health = 1;

	vx = ZOMBIE_SPEED * this->nx;
}

Zombie::~Zombie()
{
}

void Zombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_FRAMEWIDTH;
	bottom = y + ZOMBIE_FRAMEHEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
}