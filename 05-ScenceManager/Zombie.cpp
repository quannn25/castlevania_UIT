#include "Zombie.h"

Zombie::Zombie(int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ZOMBIE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->nx = nx1;

	health = 1;

	vx = ZOMBIE_SPEED * this->nx;
	this->type = eType::ZOMBIE;
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
	this->type = eType::ZOMBIE;
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

void Zombie::Render()
{
	if (health <= 0)
		return;

	bool isLeft = true;
	if (nx > 0)
		isLeft = false;

	animation_set->at(0)->Render(x, y, 255, isLeft);

	RenderBoundingBox();
}