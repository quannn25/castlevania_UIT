#include "Zombie.h"

Zombie::Zombie(int nx1)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ZOMBIE_ANI_SET_ID);
	SetAnimationSet(ani_set);

	this->nx = nx1;

	health = 1;

	vx = ZOMBIE_SPEED_X * this->nx;
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

	vx = ZOMBIE_SPEED_X * this->nx;
	this->type = eType::ZOMBIE;
}

Zombie::~Zombie()
{
}

void Zombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y + 3;
	right = x + ZOMBIE_FRAMEWIDTH;
	bottom = y + ZOMBIE_FRAMEHEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += ZOMBIE_GRAVITY * dt;

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
			listObject_Brick.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
		{
			//vx = -vx;
			//nx = -nx;
		}

		if (ny != 0)
		{
			vy = 0;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
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