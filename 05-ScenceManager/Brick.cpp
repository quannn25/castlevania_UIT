#include "Brick.h"

CBrick::CBrick(float w, float h) : CGameObject()
{
	this->width = w;
	this->height = h;
}

CBrick::CBrick(int type1) : CGameObject()
{
	if (type1 == 9) //brick ko bbox
	{
		this->width = 0;
		this->height = 0;
		return;
	}
	this->width = BRICK_BBOX_WIDTH;
	this->height = BRICK_BBOX_HEIGHT;
}

CBrick::~CBrick()
{

}

void CBrick::Render()
{

	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();

}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (this->type == eType::SPECIALBRICK)
	{
		if (this->id == 999 || this->id == 1000) // broken brick 
		{
			l = x;
			t = y - 96; // ko cho simon nhảy móc chân vào
			r = x + width;
			b = y + height;
			return;
		}
		l = x;
		t = y;
		r = x + width;
		b = y + height;
		return;
	}
	if (this->type == eType::SPECIALBRICKSMALL) // bbox nhỏ lại
	{
		l = x + 10;
		t = y + 10;
		r = x + 15;
		b = y + 15;
		return;
	}
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
