#ifndef BOOMERANGITEM_H
#define BOOMERANGITEM_H

#define BOOMERANGITEM_FRAMEWIDTH 32
#define BOOMERANGITEM_FRAMEHEIGHT 32
#define BOOMERANGITEM_GRAVITY 0.25f
#define BOOMERANGITEM_TIMEDISPLAYMAX 3000
#define BOOMERANGITEM_TIMEWAITMAX 300

#include "Item.h"
class BoomerangItem : public Item
{
public:
	BoomerangItem(float X, float Y);
	~BoomerangItem();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL); // Update và xét va chạm đất
};

#endif 