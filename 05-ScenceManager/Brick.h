#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32

#define MOVINGBRICK_SPEED_X 0.06f


class CBrick : public CGameObject
{
protected:
	float width;
	float height;

	float left_boundary;
	float right_boundary;
public:
	CBrick(int type);
	CBrick(float width, float height);
	CBrick(float left_boundary, float right_boundary, int type);
	~CBrick();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};