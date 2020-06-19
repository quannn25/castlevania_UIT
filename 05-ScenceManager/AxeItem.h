#ifndef AXEITEM_H
#define AXEITEM_H

#include "Item.h"

#define AXEITEM_FRAMEWIDTH 32
#define AXEITEM_FRAMEHEIGHT 32
#define AXEITEM_TIMEDISPLAYMAX 3000
#define AXEITEM_GRAVITY 0.25f
#define AXEITEM_TIMEWAITMAX 300

class AxeItem : public Item
{
public:
	AxeItem(float X = 0, float Y = 0);
	~AxeItem();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL); // Update và xét va chạm đất
};



#endif 