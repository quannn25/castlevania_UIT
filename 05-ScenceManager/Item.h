#ifndef ITEM_H
#define ITEM_H

#include "Sprites.h"
#include "Camera.h"
#include "Textures.h"
#include "GameObject.h"
#include "define.h"
#include "Brick.h"


class Item : public CGameObject
{
protected:
	DWORD TimeDisplayMax; // thgian tối đa cho phép hiển thị
	DWORD TimeDisplayed; // thgian đã hiển thị
	DWORD TimeWaited; // Thời gian đã chờ trước khi hiển thị
	DWORD TimeWaitMax;// Thời gian tối đa phải chờ trước khi hiển thị

	bool isFinish; // đã kết thúc chưa?

public:
	Item();
	~Item();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
	virtual void Render();
	
	bool GetFinish();
	void SetFinish(bool f);
	virtual bool isWaitingDisplay(); // đang chờ object hiển thi? - simon chưa được ăn
};

#endif