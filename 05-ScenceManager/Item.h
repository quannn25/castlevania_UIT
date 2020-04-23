#ifndef ITEM_H
#define ITEM_H

#include "Sprites.h"
#include "Camera.h"
#include "Textures.h"
#include "GameObject.h"
#include "define.h"
#include "Game.h"
#include "Brick.h"


class Item : public CGameObject
{
protected:
	int TimeDisplayMax; // thgian tối đa cho phép hiển thị
	int TimeDisplayed; // thgian đã hiển thị

	bool isFinish; // đã kết thúc chưa?

public:
	Item();
	~Item();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
	virtual void Render();
	virtual void SetReward() = 0; // update điểm or máu cho player
	
	bool GetFinish();
	void SetFinish(bool f);
};

#endif