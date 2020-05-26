#include "Enemy.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}

void Enemy::Render()
{
	bool isLeft = true;
	if (nx > 0)
		isLeft = false;
	animation_set->at(0)->Render(x, y, 255, isLeft);
	RenderBoundingBox();
}