#include "Brick.h"

CBrick::CBrick(float w, float h) : CGameObject()
{
	this->width = w;
	this->height = h;
}

CBrick::CBrick(int type1) : CGameObject()
{
	if (type1 == OBJECT_TYPE_SPECIALBRICKSMALL) //brick ko bbox
	{
		this->width = 0;
		this->height = 0;
		return;
	}
	this->width = BRICK_BBOX_WIDTH;
	this->height = BRICK_BBOX_HEIGHT;
}

CBrick::CBrick(float left_boundary, float right_boundary, int type1) : CGameObject()
{
	if (type1 == OBJECT_TYPE_MOVINGBRICK) // moving brick
	{
		this->left_boundary = left_boundary;
		this->right_boundary = right_boundary;

		this->vx = MOVINGBRICK_SPEED_X;
		return;
	}
	this->width = BRICK_BBOX_WIDTH;
	this->height = BRICK_BBOX_HEIGHT;
}

CBrick::~CBrick()
{

}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (this->type == eType::MOVINGBRICK)
	{
		vx = MOVINGBRICK_SPEED_X * nx;
		CGameObject::Update(dt);
		x += dx;

		if (x > right_boundary || x < left_boundary) // kiểm tra lại vượt biên
		{
			if (x > right_boundary)
			{
				x = right_boundary;
			}
			if (x < left_boundary)
			{
				x = left_boundary;
			}

			nx = -nx;
		}

		/*if (MainSimon::GetInstance()->GetSimon()->isOnMovingBrick)
		{
			if (MainSimon::GetInstance()->GetSimon()->movingBrick->id == this->id)
			{
				MainSimon::GetInstance()->GetSimon()->x = this->x;
			}
		}*/
		
	}
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
		if (this->id == BROKEN_CROWN1 || this->id == BROKEN_CROWN2) // broken brick 
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
	if (this->type == eType::MOVINGBRICK)
	{
		l = x;
		t = y;
		r = x + 64;
		b = y + 16;
		return;
	}
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
