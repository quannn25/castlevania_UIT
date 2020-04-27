#ifndef DAGGERITEM_H
#define DAGGERITEM_H

#include "Item.h"

#define ITEMDAGGER_TIMEDISPLAYMAX 3000
#define ITEMDAGGER_GRAVITY 0.25f
#define DAGGER_FRAMEWIDTH 32
#define DAGGER_FRAMEHEIGHT 25 //18 // buggggggggggggggggggg
#define DAGGERITEM_TIMEWAITMAX 300


class DaggerItem : public Item
{
public:
	DaggerItem(float x = 0, float y = 0);
	~DaggerItem();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL); // Update và xét va chạm đất
};
#endif 