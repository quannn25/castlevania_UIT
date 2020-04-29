#ifndef MONNEY_H
#define MONNEY_H

#include "Item.h"


#define MONEY_TIMEDISPLAYMAX 3000
#define MONEY_TIMEWAITMAX 300
#define MONEY_GRAVITY 0.25f //.25

#define MONNEY_FRAMEWIDTH 30
#define MONNEY_FRAMEHEIGHT 30




class Monney : public Item
{
public:
	Monney(float x1 = 0, float y1 = 0);
	~Monney();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL);

};

#endif