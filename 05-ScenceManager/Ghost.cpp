#include "Ghost.h"

Ghost::Ghost(int nx1)
{
	this->nx = nx1;

	health = 1;

	vx = GHOST_SPEED * this->nx;
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

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
}