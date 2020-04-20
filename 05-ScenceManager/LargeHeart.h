#pragma once
#include "Item.h"
#include "ItemManager.h"

#define LARGEHEART_GRAVITY 0.25f

#define LARGEHEART_TIMEDISPLAYMAX 5000
#define LARGEHEART_FRAMEWIDTH 24
#define LARGEHEART_FRAMEHEIGHT 20

class LargeHeart : public Item
{
public:
	LargeHeart();
	LargeHeart(float X, float Y);


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);
	virtual void SetReward();

	~LargeHeart();
};

