#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32


class CBrick : public CGameObject
{
protected:
	float width;
	float height;
public:
	CBrick(int type);
	CBrick(float width, float height);
	~CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};