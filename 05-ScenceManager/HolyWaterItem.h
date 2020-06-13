#ifndef HOLYWATERITEM_H
#define HOLYWATERITEM_H

#define HOLYWATERITEM_TIMEDISPLAYMAX 3000
#define HOLYWATERITEM_GRAVITY 0.25f
#define HOLYWATERITEM_TIMEWAITMAX 300

#define HOLYWATERITEM_FRAMEWIDTH 32
#define HOLYWATERITEM_FRAMEHEIGHT 32
#include "Item.h"
class HolyWaterItem : public Item
{
public:
	HolyWaterItem(float x1 = 0, float y1 = 0);
	~HolyWaterItem();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL); // Update và xét va chạm đất

};


#endif 