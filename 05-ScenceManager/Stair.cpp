#include "Stair.h"



Stair::Stair(float x1, float y1, int t1)
{
	x = x1;
	y = y1;
	width = 25;
	height = 25;
	t = t1;
	if (t == 1) // up
		type = eID::STAIR_UP;
}

Stair::~Stair()
{
}

void Stair::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x - width;
	right = x + width;
	top = y;
	bottom = y + height;
}

void Stair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Stair::Render()
{
	RenderBoundingBox();
}