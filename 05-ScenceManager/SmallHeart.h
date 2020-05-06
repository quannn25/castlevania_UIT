#ifndef SMALLHEART_H
#define SMALLHEART_H


#include "Item.h"

#define SMALLHEART_GRAVITY 0.04f
#define SMALLHEART_TIMEDISPLAYMAX 6000
#define SMALLHEART_TIMEWAITMAX 150

#define SMALLHEART_SPEED_X 0.05f

#define DeltaX 25 // bien dao dong

#define SMALLHEART_FRAMEWIDTH 16
#define SMALLHEART_FRAMEHEIGHT 16


class SmallHeart : public Item
{
private:
	float xBackup; // vị trí của x ban dầu

public:
	SmallHeart(float x = 0, float y = 0);
	~SmallHeart();

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *listObject = NULL);

};
#endif
