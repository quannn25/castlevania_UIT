#include "Stair.h"



Stair::Stair(float x1, float y1, int t1, int nx1)
{
	x = x1;
	y = y1;
	width = 25;
	height = 35;
	t = t1;
	nx = nx1;
}

Stair::~Stair()
{
}

void Stair::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (t == 1)
	{
		left = x - width;
		right = x + width * 2;
		top = y;
		bottom = y + height;
	}
	else
	{
		left = x - 10;
		right = x + width * 2;
		top = y;
		bottom = y + height;
	}
	/*left = x;
	right = x + width;
	top = y;
	bottom = y + height;*/
}

void Stair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Stair::Render()
{
	RenderBoundingBox();
}