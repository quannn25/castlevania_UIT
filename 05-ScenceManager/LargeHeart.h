#pragma once
#include "Item.h"

#define LARGEHEART_GRAVITY 0.005f

#define LARGEHEART_TIMEDISPLAYMAX 200
#define LARGEHEART_FRAMEWIDTH 60
#define LARGEHEART_FRAMEHEIGHT 60

class LargeHeart : public Item
{
public:
	LargeHeart();
	LargeHeart(float X, float Y);


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, CGameObject * listObject);


	~LargeHeart();
};

