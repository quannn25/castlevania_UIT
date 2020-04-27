#ifndef LARGEHEART_H
#define LARGEHEART_H
#include "Item.h"

#define LARGEHEART_GRAVITY 0.25f

#define LARGEHEART_TIMEDISPLAYMAX 5000
#define LARGEHEART_FRAMEWIDTH 24
#define LARGEHEART_FRAMEHEIGHT 20
#define LARGEHEART_TIMEWAITMAX 300

class LargeHeart : public Item
{
public:
	LargeHeart(float x = 0.0f, float y = 0.0f);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);

	~LargeHeart();
};

#endif 
