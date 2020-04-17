#ifndef ITEM_H
#define ITEM_H

#include "Sprites.h"
#include "Camera.h"
#include "Textures.h"
#include "GameObject.h"
#include "define.h"
#include "Game.h"


class Item
{
protected:
	LPANIMATION_SET animation_set;

	float x;
	float y;

	float vx;
	float vy;
	float dx;
	float dy;

	eID type;

	int TimeDisplayMax; // thời gian tối đa cho phép hiển thị.
	int TimeDisplayed; // Thời gian đã hiển thị.

	bool isFinish; // đã kết thúc chưa?

public:
	Item();
	~Item();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<CGameObject*> *listObject = NULL);
	virtual void Render();
	virtual void SetPosition(float X, float Y);
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	eID GetType();
	void RenderBoundingBox();
	bool GetFinish();
};




















#endif




