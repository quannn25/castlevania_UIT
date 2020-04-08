#include "Brick.h"


void CBrick::Render()
{


	animation_set->at(0)->Render(x, y);

}

void CBrick::Render(Camera * camera)
{

	D3DXVECTOR2 pos = camera->Transform(x, y);

	animation_set->at(0)->Render(pos.x, pos.y);

}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}