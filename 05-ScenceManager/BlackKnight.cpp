#include "BlackKnight.h"



BlackKnight::BlackKnight(int nx1)
{
	this->nx = nx1;
	health = 1; // sét máu
	vx = BLACKKNIGHT_SPEED * this->nx;
}

BlackKnight::~BlackKnight()
{
}

void BlackKnight::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BLACKKNIGHT_FRAMEWIDTH;
	bottom = y + BLACKKNIGHT_FRAMEHEIGHT;
}

void BlackKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
}